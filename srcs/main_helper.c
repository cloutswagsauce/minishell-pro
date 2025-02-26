#include "../../minishell.h"

char *handle_input(void)
{
    char *input;

    if (isatty(STDIN_FILENO))
    {
        signal_handler_interactive();
        input = readline("mini$hell 🤖: ");
    }
    else
    {
        signal_handler_non_interactive();
        input = readline("");
    }
    
    if (!input)  // Handle Ctrl+D
        return NULL;
        
    return input;
}

void execute_input(char *input, t_com **commands, t_data *data)
{
    if (*input)
    {
        *commands = parse_input(input);
        if (*commands && (*commands)->has_outpipe)
            execute_pipeline(*commands, data);
        else if (*commands)
            execute_process(*commands, data);
        finish_execution(*commands, input, data);
    }
    else
        free(input);
}