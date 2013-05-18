import java.util.*;
import java.math.BigInteger;



public class Main {

    public static void main (String[] args) {
        Scanner cin = new Scanner(System.in);
        while(cin.hasNext()) {
            BigInteger a = cin.nextBigInteger();
            BigInteger b = cin.nextBigInteger();
            BigInteger c = a.add(b);
            System.out.println(c);
        }
    }
}
