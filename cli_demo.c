#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>

typedef void(*cli_command_handler)(char* cmd);

typedef struct {
	char command[20];
	cli_command_handler handler;
	char params[30];
	char help[100];
}tds_cli_commands;

typedef struct {
	tds_cli_commands* cli_commands;
	void* next_cmd_grp;
}tds_command_group;

void Register_CLI_Command_Group(tds_cli_commands* cli_commands);
void CLI_Help_Command_Handler(char* cmd);
void Initialize_CLI();
void Display_Time(char* cmd);
void Display_Date(char* cmd);
void Error_Handler();

tds_command_group command_groups;

tds_cli_commands cli_help_commands[] = 
{
	{ "h",						CLI_Help_Command_Handler, 	"", "\r\n" },
	{ "help",					CLI_Help_Command_Handler, 	"", "help command\r\n" },
	{ "",						NULL,						"", "" }
};

tds_cli_commands cli_main_commands[] =
{
	{ "time",					Display_Time,				"", "display time\r\n"},
	{ "date",					Display_Date,				"",	"display date\r\n"},
	{ "",						NULL,						"", "" }
};


int main(void) {

	Initialize_CLI();
	Register_CLI_Command_Group(cli_main_commands);
	tds_command_group* cmd_grp = &command_groups;
	cmd_grp->cli_commands[0].handler("unused");
	cmd_grp->cli_commands[1].handler("unused");
	cmd_grp = cmd_grp->next_cmd_grp;
	cmd_grp->cli_commands[0].handler("unused");
	cmd_grp->cli_commands[1].handler("unused");
	exit(EXIT_SUCCESS);

}

void CLI_Help_Command_Handler(char* cmd) {
	tds_command_group* cmd_grp;
	tds_cli_commands* cli_commands;
	uint16_t i;
	(void)cmd;
	cmd_grp = &command_groups;
	while (cmd_grp != NULL) {
		cli_commands = cmd_grp->cli_commands;
		i = 0;
		while (cli_commands[i].handler != NULL) {
			printf("%s  %s  %s", cli_commands[i].command, cli_commands[i].params, cli_commands[i].help);
			i++;
		}
		cmd_grp = cmd_grp->next_cmd_grp;
	}
}

void Initialize_CLI() {
	command_groups.cli_commands = &cli_help_commands[0];
	command_groups.next_cmd_grp = NULL;
}

void Register_CLI_Command_Group(tds_cli_commands* cli_commands) {
	tds_command_group* cmd_grp;
	cmd_grp = &command_groups;
	while (cmd_grp->next_cmd_grp != NULL)
		cmd_grp = cmd_grp->next_cmd_grp;
	cmd_grp->next_cmd_grp = (void*)malloc(sizeof(tds_command_group));
	cmd_grp = cmd_grp->next_cmd_grp;
	if (cmd_grp != NULL) {
		cmd_grp->cli_commands = cli_commands;
		cmd_grp->next_cmd_grp = NULL;
	}
	else
		Error_Handler();
}

void Display_Time(char* cmd) {
	printf("\n %s \n", __TIME__);
}

void Display_Date(char* cmd) {
	printf("\n %s \n", __DATE__);
}

void Error_Handler() {
	printf("\n\rError while registering command to Group\n\r");
	exit(EXIT_FAILURE);
}