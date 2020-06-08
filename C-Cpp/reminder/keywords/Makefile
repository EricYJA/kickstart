Objs = keywords.o keywords_mirror.o

keywords : $(Objs)
		cc -o keywords $(Objs)

keywords_mirror.o : keywords_mirror.c
keywords.o : keywords.c  keywords.h 

clean :
		rm keywords $(Objs) 
