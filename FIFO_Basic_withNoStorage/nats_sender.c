#include <nats/nats.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LENGTH 16384
int msg_count = 0;

int
main()
{
  natsConnection* conn = NULL;
  natsStatus s;
  FILE* file;
  char line[MAX_LINE_LENGTH];
  const char* filename = "logs.json";

  // Connect to the NATS server
  s = natsConnection_ConnectTo(&conn, NATS_DEFAULT_URL);
  if (s != NATS_OK) {
    printf("Failed to connect: %s\n", natsStatus_GetText(s));
    natsConnection_Destroy(conn);
    return EXIT_FAILURE;
  }

  // Open the log file
  file = fopen(filename, "r");
  if (file == NULL) {
    perror("Failed to open file");
    natsConnection_Destroy(conn);
    return EXIT_FAILURE;
  }

  // Read and publish each line
  while (fgets(line, sizeof(line), file)) {
    // Remove the newline character if present
    line[strcspn(line, "\n")] = '\0';

    // Publish the line to the subject "logs"
    s = natsConnection_PublishString(conn, "logs", line);
    if (s != NATS_OK) {
      printf("Failed to publish: %s\n", natsStatus_GetText(s));
      fclose(file);
      natsConnection_Destroy(conn);
      return EXIT_FAILURE;
    } else {
      msg_count++;
    }
  }

  // Close the file and clean up
  fclose(file);
  natsConnection_Destroy(conn);

  printf("%d:Log lines published successfully!\n", msg_count);
  return EXIT_SUCCESS;
}
