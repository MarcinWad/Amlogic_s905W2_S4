/* AmLogic Video TEE Loader based on decompilation of original tee_preload_fw from Android Video Player */
/* Compile with: gcc tee_fw_preload.c.c -o tee_fw_preload -lteec */

#include "tee_client_api.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static const TEEC_UUID uuid = {
	0x526FC4FC,0x7EE6,0x4A12,{ 0x96, 0xe3, 0x83, 0xda, 0x95, 0x65, 0xbc, 0xe8 }
};

int main(int argc, char* argv[])
{
	TEEC_Context context;
	TEEC_Session session;
	TEEC_Operation op;
	TEEC_Result tee_rv;
	int i;

	printf("\nAmLogic TEE loading video_ucode.bin\n");

	if (argc <2 )
	{
		printf("Need video_ucode.bin path to load as a parameter!\n");
		exit(1);
	}
	else
	{
		printf("Loading file: %s\n",argv[1]);
	}
	
	memset((void *)&op, 0, sizeof(op));

	printf("Initializing context: ");
	tee_rv = TEEC_InitializeContext(NULL, &context);
	if (tee_rv != TEEC_SUCCESS) {
		printf("TEEC_InitializeContext failed: 0x%x\n", tee_rv);
		goto error;
	} else {
		printf("initialized\n");
	}

	
	
	/*
	 * Open session towards Digest TA
	 */

	printf("Openning session: ");
	tee_rv = TEEC_OpenSession(&context, &session, &uuid, 0, 0, 0, NULL);
	if (tee_rv != TEEC_SUCCESS) {
		printf("TEEC_OpenSession failed: 0x%x\n", tee_rv);
		goto error;
	} else {
		printf("TEE session opened\n");
	}
	
	FILE *fp;
	
	memset(&op, 0, sizeof(op));
	fp = fopen(argv[1], "r");
	if (!fp)
	{
		printf("Video firmare file not found!\n Exiting.\n");
		exit(1);
	}
	
	
	fseek(fp, 0, SEEK_END);
	int filesize = ftell(fp);
	printf("filesize: %d\n",filesize);
	fseek(fp, 0LL, 0LL);
	void *filedata = malloc(filesize);
	fread(filedata, filesize, 1, fp);
	
	op.started = 0;
	op.paramTypes = TEEC_PARAM_TYPES(TEEC_MEMREF_TEMP_INPUT,
					 TEEC_MEMREF_TEMP_INPUT		,
					 TEEC_VALUE_INPUT, TEEC_NONE);
			 
	op.params[0].tmpref.buffer = filedata+256;
	op.params[0].tmpref.size = filesize-256;
	
	op.params[1].tmpref.buffer = filedata;
	op.params[1].tmpref.size = 256;
	
	tee_rv = TEEC_InvokeCommand(&session, 0, &op, 0);
	if (tee_rv != TEEC_SUCCESS) {
		printf("TEEC_InvokeCommand failed: 0x%x\n", tee_rv);
		goto error;
	} else {
		printf("Video firmware loaded\n");
	}
	
	
	error:
	TEEC_CloseSession(&session);
	TEEC_FinalizeContext(&context);
	exit(tee_rv);
}
