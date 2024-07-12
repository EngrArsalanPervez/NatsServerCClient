#include <nats/nats.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int msg_count = 0;

// Callback to handle received messages
void
onMessage(natsConnection* nc,
          natsSubscription* sub,
          natsMsg* msg,
          void* closure)
{
  msg_count++;
  printf("%d: Received message: %s\n", msg_count, natsMsg_GetData(msg));
  natsMsg_Destroy(msg);
}

int
main()
{
  natsConnection* conn = NULL;
  natsSubscription* sub = NULL;
  natsStatus s;

  // Connect to the NATS server
  s = natsConnection_ConnectTo(&conn, NATS_DEFAULT_URL);
  if (s != NATS_OK) {
    printf("Failed to connect: %s\n", natsStatus_GetText(s));
    natsConnection_Destroy(conn);
    return EXIT_FAILURE;
  }

  // Subscribe to the subject "logs"
  s = natsConnection_Subscribe(&sub, conn, "logs", onMessage, NULL);
  if (s != NATS_OK) {
    printf("Failed to subscribe: %s\n", natsStatus_GetText(s));
    natsSubscription_Destroy(sub);
    natsConnection_Destroy(conn);
    return EXIT_FAILURE;
  }

  // Keep the connection alive to receive messages
  printf("Listening for messages on subject 'logs'...\n");
  while (true) {
    nats_Sleep(1000);
  }

  // Clean up (this part will never be reached in this example)
  natsSubscription_Destroy(sub);
  natsConnection_Destroy(conn);

  return EXIT_SUCCESS;
}
