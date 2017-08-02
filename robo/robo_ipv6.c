// Compile with: gcc /Programme/GPIO.c -o /Programme/GPIO

#include <sys/types.h>
#include <sys/stat.h>
#include <pthread.h>
#include <fcntl.h>
#include <termios.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <wiringPi.h>
#include <inttypes.h>

#define TRIG_PIN 1
#define ECHO_PIN 0
uint32_t dead = 0;
#define maxdead 20000
int fd, fd2;

char GPIO[4][30] = {    "/sys/class/gpio/gpio23/value", // Array für die GPIOs
                        "/sys/class/gpio/gpio25/value",
                        "/sys/class/gpio/gpio24/value",
                        "/sys/class/gpio/gpio8/value"
                   };

char dir[5][4][2] = {    {"0", "0", "0", "0"},  //STOP  = 0
    {"0", "1", "0", "1"},   //ZURÜCK= 1
    {"1", "0", "1", "0"},   //VOR   = 2
    {"1", "0", "0", "1"},   //LINKS = 3
    {"0", "1", "1", "0"}
};  //RECHTS= 4

char Input;
int i;
int res, x = 0;

struct msg {
    char cmd[256];
    int val;
    int loop;
};

int gpiowrite(int d)
{
    int i;
    if (0 <= d && d < 5) {
        for (i = 0; i < 4; i++)
        {
            fd = open(GPIO[i], O_WRONLY | O_NOCTTY);// Datei mit der Indexnummer "i-1" öffnen und Name unter "fd" speichern
            res = write(fd, dir[d][i], 1);      // Argument mit der Nummer "i" in die geöffnete Datei schreiben
            close (fd);             // Datei schließen
        }
        return 1;
    }
    else return 0;
}

void setup()
{
    wiringPiSetup();
    pinMode(TRIG_PIN, OUTPUT);
//        pinMode(LED, OUTPUT);
    pinMode(ECHO_PIN, INPUT);

    //TRIG_PIN pin must start LOW
    digitalWrite(TRIG_PIN, LOW);
    delay(30);
}

uint16_t getCM()
{
    //Send TRIG_PIN pulse
    delayMicroseconds(3000);
    digitalWrite(TRIG_PIN, HIGH);
    delayMicroseconds(20);
    digitalWrite(TRIG_PIN, LOW);

    //Wait for ECHO_PIN start
    dead = 0;
    while (digitalRead(ECHO_PIN) == LOW) {dead++; if (dead >= maxdead)break;};

    //Wait for ECHO_PIN end
    long startTime = micros();
    dead = 0;
    while (digitalRead(ECHO_PIN) == HIGH) {dead++; if (dead >= maxdead)break;};
    long travelTime = micros() - startTime;

    //Get distance in cm
    //distance = (travelTime / 2 * 1000000[s]) * (331,5 * 100[cm])
    //distance = (travelTime * 331,5) / 20000
    //--> 331.5/20000 =~ 1/58

    int distance = travelTime / 58;

    return distance;
}

int newsockfd;
void *doprocessing (void *arg)
{
    int n;
    struct msg buffer;

    while (1) {
        bzero(&buffer, sizeof(buffer));

        n = read(newsockfd, &buffer, sizeof(buffer));
        if (n < 0)
        {
            perror("ERROR reading from socket");
            exit(1);
        }

        if (strncmp(buffer.cmd, "DVOR", 4) == 0)
        {   gpiowrite(1);
            //sleep(1);
            //  printf("%s\n",buffer.cmd);
        }
        else if (strncmp(buffer.cmd, "DLIN", 4) == 0)
        {   gpiowrite(3);
            //sleep(1);
            //  printf("%s\n",buffer.cmd);
        }
        else if (strncmp(buffer.cmd, "DREC", 4) == 0)
        {   gpiowrite(4);
            //sleep(1);
            //  printf("%s\n",buffer.cmd);
        }
        else if (strncmp(buffer.cmd, "DRUC", 4) == 0)
        {   gpiowrite(2);
            //sleep(1);
            //  printf("%s\n",buffer.cmd);
        }

        else {
            gpiowrite(0);
            //sleep(1);
            //  printf("%s\n",buffer.cmd);
        }
//   printf("Here is the message:%s\n",buffer.cmd);
    }
}

void *uschall (void *arg)
{
    int n;
    struct msg buffer;
    uint16_t dist;
    while (1) {
        bzero(&buffer, sizeof(buffer));
        dist = getCM();
        sprintf(buffer.cmd, "%.2d", dist);
        //printf("%.2d\n", dist);
        //strcpy(buffer.cmd,"moep\n");
        //write(newsockfd, dist, sizeof(dist));
        //    n = read(newsockfd,buffer,sizeof(buffer));
        n = write(newsockfd, &buffer, sizeof(buffer));
        if (n < 0) {error("ERROR writing to socket");}
    }
}

int main( int argc, char *argv[] )
{
    int sockfd, portno, clilen;
    struct msg buffer;
    struct sockaddr_in6 serv_addr, cli_addr;
    int  n, pid;
    char* strings[] = {"hello ", "world\n"};
    pthread_t thread1, thread2;
    char client_addr_ipv6[100];
//pthread_init();
    setup();

    /* First call to socket() function */
    printf("\nIPv6 TCP Server Started...\n");
    sockfd = socket(AF_INET6, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        perror("ERROR opening socket");
        exit(1);
    }
    /* Initialize socket structure */
    bzero((char *) &serv_addr, sizeof(serv_addr));
    portno =  5006;
    serv_addr.sin6_flowinfo = 0;
    serv_addr.sin6_family = AF_INET6;
    serv_addr.sin6_addr = in6addr_any;
    serv_addr.sin6_port = htons(portno);

    /* Now bind the host address using bind() call.*/
    if (bind(sockfd, (struct sockaddr *) &serv_addr,
             sizeof(serv_addr)) < 0)
    {
        perror("ERROR on binding");
        exit(1);
    }
    /* Now start listening for the clients, here
     * process will go in sleep mode and will wait
     * for the incoming connection
     */
    listen(sockfd, 5);
    clilen = sizeof(cli_addr);
    printf("warte auf Verbindung an PORT: %d  ... :p\n", portno);
    while (1)
    {
        newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
        if (newsockfd < 0)
        {
            perror("ERROR on accept");
            exit(1);
        }
        else
        {
            inet_ntop(AF_INET6, &(cli_addr.sin6_addr), client_addr_ipv6, 100);
            printf("Incoming connection from client having IPv6 address: %s\n", client_addr_ipv6);
        }
        /* Create thread */
        pthread_create(&thread1, NULL, doprocessing, strings);
        pthread_create(&thread2, NULL, uschall, strings);
    } /* end of while */
}








