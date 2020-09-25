#include <stdlib.h>
#include <stdio.h>
#include <netinet/in.h>
#include <netdb.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>
#include <inttypes.h>
#include <time.h>

typedef struct {
  uint16_t xid;      /* Randomly chosen identifier */
  uint16_t flags;    /* Bit-mask to indicate request/response */
  uint16_t qdcount;  /* Number of questions */
  uint16_t ancount;  /* Number of answers */
  uint16_t nscount;  /* Number of authority records */
  uint16_t arcount;  /* Number of additional records */
} dns_header_t;

typedef struct {
  char *name;        /* Pointer to the domain name in memory */
  uint16_t dnstype;  /* The QTYPE (1 = A) */
  uint16_t dnsclass; /* The QCLASS (1 = IN) */
} dns_question_t;

typedef struct {
  uint16_t compression;
  uint16_t type;
  uint16_t class;
  uint32_t ttl;
  uint16_t length;
  uint16_t preference;
  char name[256] ;
} __attribute__((packed)) dns_record_a_t;


/*
  argv[1] = hostname
  argv[2] = DNS IP
*/
int main(int argc, char **argv){
    srand(time(NULL));
    int socketfd = socket (AF_INET, SOCK_DGRAM, 0),
      xid = rand()%256, argv1_len = strlen(argv[1]);
    struct sockaddr_in address;
    struct in_addr addr;
    inet_aton(argv[2], &addr);
    address.sin_family = AF_INET;
    /* OpenDNS is currently at 208.67.222.222 (0xd043dede) */
    /* here we put the argv[2]*/
    address.sin_addr = addr;
    /* DNS runs on port 53 */
    address.sin_port = htons (53);

    /* Set up the DNS header */
    dns_header_t header;
    memset (&header, 0, sizeof (dns_header_t)); /* set all the struct with zeroes */
    header.xid= htons (xid);    /* Randomly chosen ID */
    header.flags = htons (0x0100); /* Q=0, RD=1 */
    header.qdcount = htons (1);    /* Sending 1 question */


    /* Set up the DNS question */
    dns_question_t question;
    question.dnstype = htons (15);  /* QTYPE 1=A,15 = MX */
    question.dnsclass = htons (1); /* QCLASS 1=IN */

    //Traverse through the name, looking for the . locations 
    int dot_count = 0,char_count=0;
    char part_host_name[256],array_name[256];
    memset (&array_name, 0, sizeof array_name);
    for(int i = 0; i<=argv1_len;i++)
    {
      if(argv[1][i] == '.' || i == argv1_len)
      {

        part_host_name[char_count] = 0;
        // set first position of the substring the length and add the substring
        array_name[i - char_count] = char_count;
        strcat(array_name,part_host_name);
        // erase char_count to take another substring
        dot_count++;
        char_count = 0; 
      }else
      {
        // take the string before the dot
        part_host_name[char_count] = argv[1][i];
        char_count++;
      }
    }

    /* DNS name format requires dot_count + 1 bytes more than the length of the
    domain name as a string */
    question.name = calloc (argv1_len + dot_count, sizeof (char));
    memcpy(question.name, array_name,strlen(array_name));

    /*
    Assembling the DNS header and question to send via a UDP packet
    */

    /* Copy all fields into a single, concatenated packet */
    size_t packetlen = sizeof (header) + argv1_len + dot_count +
    sizeof (question.dnstype) + sizeof (question.dnsclass);
    uint8_t *packet = calloc (packetlen, sizeof (uint8_t));
    uint8_t *p = (uint8_t *)packet;

    /* Copy the header first */
    memcpy (p, &header, sizeof (header));
    p += sizeof (header);

    /* Copy the question name, QTYPE, and QCLASS fields */
    memcpy (p, question.name, argv1_len + 2);
    p += argv1_len + 2;
    memcpy (p, &question.dnstype, sizeof (question.dnstype));
    p += sizeof (question.dnstype);
    memcpy (p, &question.dnsclass, sizeof (question.dnsclass));

    /* Send the packet to OpenDNS, then request the response */
    sendto (socketfd, packet, packetlen, 0, (struct sockaddr *) &address, 
            (socklen_t) sizeof (address));


    socklen_t length = 0;
    uint8_t response[512];
    memset (&response, 0, 512);

    printf("antes\n");
    /* Receive the response from OpenDNS into a local buffer */
    ssize_t bytes = recvfrom (socketfd, response, 512, 0,
                            (struct sockaddr *) &address, &length);
    
    printf("depois\n");
    printf("Bytes: %ld\n",bytes);
    printf("Response: %s\n", response);


    dns_header_t *response_header = (dns_header_t *)response;

    uint8_t *start_of_name = (uint8_t *) (response + sizeof (dns_header_t));
    uint8_t total = 0;
    uint8_t *field_length = start_of_name;
    while (*field_length != 0)
    {
        /* Restore the dot in the name and advance to next length */
        total += *field_length + 1;
        *field_length = '.';
        field_length = start_of_name + total;
    }
    *field_length = '\0'; /* Null terminate the name */
    dns_record_a_t *records = (dns_record_a_t *) (field_length + 5);
    printf("%d\n",ntohs (response_header->ancount));

    for (int i = 0; i < ntohs (response_header->ancount); i++)
    {
      printf ("TYPE: %" PRId16 "\n", ntohs (records[i].type));
      printf ("CLASS: %" PRId16 "\n", ntohs (records[i].class));
      printf ("TTL: %" PRIx32 "\n", ntohl (records[i].ttl));
      //printf ("IPv4: %08" PRIx32 "\n", ntohl (records[i].addr));
      printf ("IPv4: %s\n", records[i].name);
    }
    free(question.name);
    return 0;
}