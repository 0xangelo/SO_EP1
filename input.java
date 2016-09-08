import edu.princeton.cs.algs4.*;
public class input {

    public static void main(String[] args) {
        Out out;
        int N = Integer.parseInt(args[0]);
        int proc;
        double dt, deadline, t0;
        for (int j = 0; j < 30; j++) {
            String name = new String ();
            name = "teste" + (j+1) + ".txt";
            out = new Out (name);
            for (int i = 0; i < N; i++) {
                dt = 0.2 + StdRandom.uniform(15)*0.2;
                t0 = (i/3)*0.2;
                deadline = t0 + dt + StdRandom.uniform(10)*0.2;
                out.printf("%f processo%d %f %f\n", t0, i, dt, deadline);            
            }
            out.close ();
        }
    }
}