#include "../../minishell.h"

/*void for_each_item(t_list *tmp, int i, char **envp)
{
    size_t len;

    int i;
    i = 0;
    while (tmp)
    {
        len = ft_strlen(tmp->name) + ft_strlen(tmp->value) + 2; 
        envp[i] = malloc(len);
        if (!envp[i])
        {
            while (i > 0)
                free(envp[--i]);
            free(envp);
            return (NULL);
        }
        ft_strlcpy(envp[i], tmp->name, len);          
        ft_strlcat(envp[i], "=", len);               
        ft_strlcat(envp[i], tmp->value, len);        
        tmp = tmp->next;
        i++;
    }
}*/


char **env_list_to_envp(t_list *env_list)
{
    int count;
    t_list *tmp;
	char **envp;
    size_t len;

    

    count = 0;
    tmp = env_list;

    while (tmp)
    {
        count++;
        tmp = tmp->next;
    }
    envp = malloc(sizeof(char *) * (count + 1));
    if (!envp)
        return (NULL);
    tmp = env_list;
    int i = 0;
    while (tmp)
    {
        len = ft_strlen(tmp->name) + ft_strlen(tmp->value) + 2; 
        envp[i] = malloc(len);
        if (!envp[i])
        {
            while (i > 0)
                free(envp[--i]);
            free(envp);
            return (NULL);
        }
        ft_strlcpy(envp[i], tmp->name, len);          
        ft_strlcat(envp[i], "=", len);               
        ft_strlcat(envp[i], tmp->value, len);        
        tmp = tmp->next;
        i++;
    }
    envp[i] = NULL;
    return (envp);
}
