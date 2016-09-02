



import edu.princeton.cs.algs4.*;
public class input {

    public static void main(String[] args) {
        int N = Integer.parseInt(args[0]);
        int proc;
        int p1, p3;


        for (int i = 0; i < N; i++) {
            p1 = 2 + StdRandom.uniform(10);
            p3 = StdRandom.uniform(50) + p1;
            StdOut.printf("%3d processo%d %3d %3d\n", i, i, p1,  p3);
        }

    }


}
