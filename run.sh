#! /bin/bash

RUN(){
    gnome-terminal --execute bash -c "RUN_QST;bash"
}



RUN_QST (){
    cd "/opt/msv/group-5/bin"
    ./qstlink2
}

export -f RUN_QST

RUN
