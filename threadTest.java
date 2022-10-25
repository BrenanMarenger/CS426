//Brenan Marenger
//http://euclid.nmu.edu/~rappleto/Classes/CS426/Threads/Java/CalcPI/CalcPI3.java
//https://stackoverflow.com/questions/180158/how-do-i-time-a-methods-execution-in-java

class Collatz {
	public static void main(String[] args) {

		int totalThread = 8;

		MyThread threads[] = new MyThread[totalThread];
		Previous prev = new Previous(new int[10000000]); // array with 10mil index, each index holds sequence if
														 // possible
		getLargest largest = new getLargest(0, 0);

		long start = System.nanoTime();
		for (int i = 0; i < totalThread; i++) {
			threads[i] = new MyThread(i, totalThread, largest, prev);
			threads[i].start();
		}

		try {
			for (int i = 0; i < totalThread; i++) {
				threads[i].join();
			}
		} catch (InterruptedException e) {
		}
		long end = System.nanoTime();

		long duration = (end - start) / 1000000;
		System.out.println("Calculation time: " + duration + "ms");
		System.out.println("Number: " + largest.largestNum);
		System.out.println("Largest Sequence: " + largest.largestSeq);
	}
}

class MyThread extends Thread {
	int totalThread;
	int counter;
	getLargest large;
	Previous prev;

	MyThread(int i, int j, getLargest large, Previous prev) {
		counter = i + 1;
		System.out.println("Creating thread #" + counter);
		totalThread = j;
		this.large = large;
		this.prev = prev;
	}

	int largestSeq = 0;
	int largestSeqNum = 0;
	long temp = 0;
	int sequence = 0;

	public void run() {
		while (counter <= 10000000) {
			temp = counter;
			sequence = 0;
			while (temp != 1) {

				if (temp < counter) {// prev[i] !=0
					synchronized (prev) { // (prev.prev[(int) temp]
						sequence += prev.prev[(int) temp];// reading from prev
					}
					break;
				}
				
				if (temp % 2 == 1) {
					temp = (3 * temp) + 1;
					sequence++;
				} else if (temp % 2 == 0) {
					temp = temp / 2;
					sequence++;
				}
				
			}

			// only one thread can write at a time
			if (counter < 10000000) {
				synchronized (prev) {
					prev.prev[(int) temp] = sequence; // writing to prev
				}
			}

			if (sequence > large.largestSeq) {
				synchronized (large) {
					large.largestNum = counter; // writing to large
					large.largestSeq = sequence;
				}
			}
			counter = counter + totalThread;
		}
	}
}

class getLargest {
	int largestNum;
	int largestSeq;

	getLargest(int index, int sequence) {
		largestNum = index;
		largestSeq = sequence;
	}
}

class Previous {
	int prev[];

	Previous(int[] prev) {
		this.prev = prev;
	}
}
