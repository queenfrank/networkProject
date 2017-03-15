#include"utili.h"

void Process_Handler(int sockConn);

void Process_Handler(int sockConn){
    OperStruct op; 
    int result;
    while(1){
        int res = recv(sockConn, &op, sizeof(op), 0); 
        if(res == -1){
            printf("recv data fail.\n");
            continue;
        }   
        if(op.oper == ADD){
            result = op.op1 + op.op2;
        }else if(op.oper == SUB)
        {   
            result = op.op1 - op.op2;
        }else if(op.oper == MUL){
            result = op.op1 * op.op2;
        }else if(op.oper == DIV){
            result = op.op1 / op.op2;
        }else if(op.oper == QUIT){
            break;
        }   

        res = send(sockConn, &result, sizeof(result), 0); 
        if(res == -1){
            printf("send data fail.\n");
            continue;
        }
    }
    close(sockConn);
}

int main(void){
    int sockSer = socket(AF_INET, SOCK_STREAM, 0);
    if(sockSer == -1){
        perror("socket");
        return -1;
    }
    struct sockaddr_in addrSer, addrCli;
    addrSer.sin_family = AF_INET;
    addrSer.sin_port = htons(SERVER_PORT);
    addrSer.sin_addr.s_addr = inet_addr(SERVER_IP);

    socklen_t len = sizeof(struct sockaddr);
    int res = bind(sockSer, (struct sockaddr*)&addrSer, len);
    if(res == -1){
        perror("bind");
        close(sockSer);
        return -1;       
   }

    listen(sockSer, LISTEN_QUEUE);

    int sockConn;
    while(1){
        printf("Server Wait Client Connect.......\n");
        sockConn = accept(sockSer, (struct sockaddr*)&addrCli, &len);
        if(sockConn == -1){
            printf("Server Accept Client Connect Fail.\n");
            continue;
        }else{
            printf("Server Accept Client Connect Success.\n");
            printf("Client IP:>%s\n", inet_ntoa(addrCli.sin_addr));
            printf("Client Port:>%d\n",ntohs(addrCli.sin_port));
        }

        pid_t pid;
        pid = fork();
        if(pid == 0){
            close(sockSer);
            Process_Handler(sockConn);
            exit(0);
        }else if(pid > 0){
            close(sockConn);
            continue;    
        }else{
            printf("Create Process Fail.\n");
            continue;
        }
    }
    close(sockSer);
    return 0;
}