FLAGS=-g

mdp: mdp.c mdp.h
	gcc ${FLAGS} -c mdp.c

start: mdp
	gcc ${FLAGS} -o start start.c mdp.o

transition: mdp
	gcc ${FLAGS} -o transition transition.c mdp.o

utilities: mdp utilities.c utilities.h
	gcc ${FLAGS} -c utilities.c

value: mdp value_iteration.c
	gcc ${FLAGS} -o value_iteration value_iteration.c  mdp.o utilities.o

policy: mdp policy_iteration.c policy_evaluation.c
	gcc ${FLAGS} -c policy_evaluation.c 
	gcc ${FLAGS} -o policy_iteration policy_iteration.c  \
	mdp.o utilities.o policy_evaluation.o

tidy: 
	rm *~

clean: 
	rm *.o
	rm value_iteration
