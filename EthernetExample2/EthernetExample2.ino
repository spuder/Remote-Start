
#include <Ethernet.h>
#include <SPI.h>
boolean reading = false;

//Thanks to ERIK at open blackboard for the master branch of this code
//http://metatek.blogspot.com/2011/07/controlling-led-from-web-page-with.html

// Updated by Spuder 
// spuder.wordpress.com
// March 3, 2012



// Enter a MAC address and IP address for your controller below.
// The IP address will be dependent on your local network:
byte mac[] = { 0x90, 0xA2, 0xDA, 0x00, 0x49, 0xD6 };
byte ip[] = { 172,28,0,18 };
static char baseurl[]="http://172.28.0.18/";

// Before the release of arduino 1.0 you created a new Server object like this.

// Server aServer = Server(80); //port 80

// With the updated Arduino 1.0 library you create a new Server object like this.
EthernetServer aServer(80); //server port


void setup()
{
  //Pins 10,11,12 & 13 are used by the ethernet shield
  pinMode(8, OUTPUT);

  Ethernet.begin(mac, ip);
  aServer.begin();
}






void loop()
{
  // listen for incoming clients, and process qequest.
  checkForClient();
}
 
 
 
 
 
void checkForClient()
{

  // Create a new client object of type EthernetClient called aClient
  // Previous to arduino 1.0 the Client Object was created like this
  // Client aClient = new Client();
  EthernetClient aClient = aServer.available();

  // If we have someone connected to the arduino server
  if (aClient) 
  {

    // an http request ends with a blank line
    boolean currentLineIsBlank = true;
    boolean sentHeader = false;

    //As long as that connection is active
    while (aClient.connected()) 
    {
      // If the client is available? Not sure what this means?
      if (aClient.available()) 
      {
        if(!sentHeader)
        {
          // send a standard http response header
          aClient.println("HTTP/1.1 200 OK");
          aClient.println("Content-Type: text/html");
          aClient.println();
          aClient.println("LED switch");
          
          aClient.println("<form METHOD=get action=\"");
          aClient.println(baseurl);
          aClient.println("\">");
          aClient.println("<input type=hidden name=LED value=0>");
          aClient.println("<input type=submit value=\"off\"></form>");
          
          aClient.println("<form METHOD=get action=\"");
          aClient.println(baseurl);
          aClient.println("\">");
          aClient.println("<input type=hidden name=LED value=1>");
          aClient.println("<input type=submit value=\"on\"></form>");
          sentHeader = true;

        }//end if (!sentHeader)




        // Listen for the characters that are returned by the client
        // The url will change to something like 192.168.1.1/?1
        // We are watching for the character that appears after the ? and saving that character to a char named c
        // We then compare that char c against our logic. 
        char c = aClient.read();
        if(reading && c == ' ') reading = false;
        if(c == '?') reading = true; //found the ?, begin reading the info
        if(reading)
        {
            Serial.print(c);
            if(c == '0') 
            {
               digitalWrite(8, LOW);
             
               break;
            }
            if(c == '1') 
            {
               digitalWrite(8, HIGH);
                 delay(500);
               digitalWrite(8, LOW);
               break;
            }
         }//end if(reading)
         
        if (c == '\n' && currentLineIsBlank)  break;

        if (c == '\n') 
        {
          currentLineIsBlank = true;
        }

        else if (c != '\r') 
        {
          currentLineIsBlank = false;
        }


      }// end   if (aClient.available())

    }// end   while (aClient.connected())  


    delay(1); // give the web browser time to receive the data
    aClient.stop(); // close the connection:
    
    
  } // end  if (aClient) 
 
}// end   checkForClients()

