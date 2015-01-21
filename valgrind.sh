valgrind -v --leak-check=full --show-reachable=yes --log-file=valgrind.log --track-origins=yes     \
    ./main 3 984

