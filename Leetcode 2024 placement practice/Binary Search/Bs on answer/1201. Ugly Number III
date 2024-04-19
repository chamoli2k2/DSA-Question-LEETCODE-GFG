class Solution {
public:
    int nthUglyNumber(int n, int A, int B, int C) {
        // Logic (Binary Search)
        // We can find the kth ugly element by binary searching it's position
        // No upto N divisible by a = N / a
        // No upto N divisible by b = N / b
        // No upto N divisible by c = N / c
        // No upto N divisible by both (a,b) = N / Lcm(a,b)
        // No upto N divisible by both (b,c) = N / Lcm(b,c)
        // No upto N divisible by both (c,a) = N / Lcm(c,a)
        // No upto N divisible by (a,b,c) = N / Lcm(a,b,c)

        // We can find the position of any ugly number by this
        // We already have range of ugly number from constraint
        
        long long s = 1;
        long long e = 2 * 1e9;
        long long ans;
        // Type converting
        long a = long(A);
        long b = long(B);
        long c = long(C);

        while(s <= e){
            long long mid = s + (e - s) / 2;
            long long count = 0;
            // Finding the position of ugly number
            // (A U B U C) = A + B + C - (A ∩ B) - (B ∩ C) - (C ∩ A) + (A ∩ B ∩ C)
            count += mid / a;
            count += mid / b;
            count += mid / c;
            count -= mid / lcm(a,b);
            count -= mid / lcm(b,c);
            count -= mid / lcm(c,a);
            count += mid / lcm(a,lcm(b,c));

            if(count >= n){
                ans = mid;
                e = mid - 1;
            }
            else{
                s = mid + 1;
            }
        }

        return ans;
    }
};