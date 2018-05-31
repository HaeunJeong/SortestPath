hw5: final.o HW5_Graph.o HW5_Heap.o 
	gcc -o hw5 final.o HW5_Graph.o HW5_Heap.o 
 
final.o: HW5_Heap.h HW5_Graph.h final.c
HW5_Heap.o: HW5_Heap.h HW5_Heap.c
HW5_Graph.o: HW5_Graph.h HW5_Graph.c

clean:
	rm -f *.o