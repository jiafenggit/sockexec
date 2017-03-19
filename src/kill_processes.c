#include "common.h"

void kill_processes(force)
int force;
{
    unsigned int i;
    tain_t c_now;
    int signal = SIGTERM;
    for(i=0; i < conn_tbl_len; i++)
    {
        if(force || (timeout > 0 && tain_lesseq(&(conn_tbl[i].deadline),now)))
        {
            if(conn_tbl[i].child_pid > 0)
            {
                if(conn_tbl[i].sigsent)
                {
                    signal = SIGKILL;
                }
                else
                {
                    tain_now(&c_now);
                    tain_addsec(&(conn_tbl[i].deadline),&c_now,kill_timeout);
                    if(deadline == 0 || tain_less(&(conn_tbl[i].deadline),deadline))
                    {
                        deadline = &(conn_tbl[i].deadline);
                    }
                }

                LOLDEBUG("kill_processes: sending %s to %d",sig_name(signal),conn_tbl[i].child_pid);
                kill(conn_tbl[i].child_pid, signal);
                conn_tbl[i].sigsent = 1;
            }
            else if(conn_tbl[i].client > -1)
            {
                close_connection(i,0);
            }
        }
        else
        {
            if(deadline == 0 || (timeout > 0 && tain_less(deadline,&(conn_tbl[i].deadline))))
            {
                LOLDEBUG("kill_processes: setting new global deadline");
                deadline = &(conn_tbl[i].deadline);
            }
        }
    }
}
