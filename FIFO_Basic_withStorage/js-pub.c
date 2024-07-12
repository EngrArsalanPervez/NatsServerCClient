#include <nats/nats.h>
#include <stdio.h>
#include <string.h>

#define NATS_STREAM_NAME "pace2"
#define NATS_SUBJ_DPI "dpi"
#define NATS_SUBJ_LLDP "lldp"

natsConnection* conn = NULL;
jsCtx* js = NULL;
jsOptions jsOpts;
jsErrCode jerr = 0;
natsStatus s;

void
publishMsg(char* payload)
{
  int dataLen = (int)strlen(payload);
  jsPubAck* pa = NULL;
  s = js_Publish(
    &pa, js, NATS_SUBJ_DPI, (const void*)payload, dataLen, NULL, &jerr);
  if (s == NATS_OK) {
    jsPubAck_Destroy(pa);
  }
}

void
natsInitialize(void)
{
  s = natsConnection_ConnectTo(&conn, NATS_DEFAULT_URL);
  if (s != NATS_OK) {
    printf("ERROR_NATS:Failed to connect: %s\n", natsStatus_GetText(s));
    natsConnection_Destroy(conn);
    exit(1);
  }

  s = jsOptions_Init(&jsOpts);
  if (s != NATS_OK) {
    printf("ERROR_NATS:jsOptions_Init: %s\n", natsStatus_GetText(s));
    natsConnection_Destroy(conn);
    exit(1);
  }

  s = natsConnection_JetStream(&js, conn, &jsOpts);
  if (s == NATS_OK) {
    jsStreamInfo* si = NULL;
    s = js_GetStreamInfo(&si, js, NATS_STREAM_NAME, NULL, &jerr);
    if (s == NATS_NOT_FOUND) {
      printf("ERROR_NATS:Failed to connect: %s\n", natsStatus_GetText(s));
      natsConnection_Destroy(conn);
      exit(1);
    }
    if (s == NATS_OK) {
      jsStreamInfo_Destroy(si);
    }
  }
}

void
natsDestroy(void)
{
  jsCtx_Destroy(js);
  natsConnection_Destroy(conn);
  nats_Close();
}

int
main(int argc, char** argv)
{
  natsInitialize();

  FILE* file = fopen("logs.json", "r");
  if (file == NULL) {
    perror("Failed to open file");
    return EXIT_FAILURE;
  }

  size_t len = 0;
  size_t read;
  char* line = NULL;

  while ((read = getline(&line, &len, file)) != -1) {
    publishMsg(line);
  }
  fclose(file);

  natsDestroy();
  return 0;
}
