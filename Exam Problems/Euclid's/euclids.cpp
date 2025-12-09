#include <iostream>
#include <vector>
using namespace std;

struct EuclidStep {
    long long a;
    long long b;
    long long quotient;
    long long remainder;
};

int main() {
    long long a, m;
    
    cout << "Enter the number to find the inverse of: ";
    cin >> a;
    cout << "Enter the modulus: ";
    cin >> m;
    
    long long original_a = a;
    long long original_m = m;
    
    vector<EuclidStep> steps;
    
    // Step 1: Run Euclid's algorithm forward
    cout << "\n=== Step 1: Euclid's Algorithm (Forward) ===" << endl;
    cout << "Finding gcd(" << a << ", " << m << "):\n" << endl;
    
    long long temp_a = a;
    long long temp_b = m;
    
    // Make temp_a the larger one
    if (temp_a < temp_b) {
        swap(temp_a, temp_b);
    }
    
    while (temp_b != 0) {
        long long q = temp_a / temp_b;
        long long r = temp_a % temp_b;
        
        cout << temp_a << " = " << q << " x " << temp_b << " + " << r << endl;
        
        EuclidStep step = {temp_a, temp_b, q, r};
        steps.push_back(step);
        
        temp_a = temp_b;
        temp_b = r;
    }
    
    long long gcd = temp_a;
    cout << "\ngcd(" << original_a << ", " << original_m << ") = " << gcd << endl;
    
    if (gcd != 1) {
        cout << "\nNo multiplicative inverse exists because gcd != 1" << endl;
        return 0;
    }
    
    cout << "\nGood - the inverse exists since gcd = 1!" << endl;
    
    // Step 2: Extended Euclidean Algorithm (work backwards)
    cout << "\n=== Step 2: Work Backwards (Extended Euclidean Algorithm) ===" << endl;
    cout << "Expressing 1 as a linear combination:\n" << endl;
    
    int n = steps.size();
    
    // Find the step that produced remainder 1 (second to last step)
    int start_idx = n - 2;
    
    cout << "Starting from: 1 = " << steps[start_idx].a << " - " << steps[start_idx].quotient << " x " << steps[start_idx].b << endl;
    
    // Track: 1 = coef1 * val1 + coef2 * val2
    // where val1 is the larger value, val2 is the smaller
    long long coef1 = 1;
    long long val1 = steps[start_idx].a;
    long long coef2 = -steps[start_idx].quotient;
    long long val2 = steps[start_idx].b;
    
    cout << "1 = (" << coef1 << ") x " << val1 << " + (" << coef2 << ") x " << val2 << endl;
    
    // Work backwards through remaining steps
    for (int i = start_idx - 1; i >= 0; i--) {
        // Currently: 1 = coef1 * val1 + coef2 * val2
        // From step i: steps[i].remainder = steps[i].a - steps[i].quotient * steps[i].b
        // Also: val2 = steps[i].remainder and val1 = steps[i].b
        
        // Substituting val2:
        // 1 = coef1 * steps[i].b + coef2 * (steps[i].a - steps[i].quotient * steps[i].b)
        // 1 = coef2 * steps[i].a + (coef1 - coef2 * steps[i].quotient) * steps[i].b
        
        cout << "\nSubstitute " << val2 << " = " << steps[i].a << " - " << steps[i].quotient << " x " << steps[i].b << ":" << endl;
        
        long long new_coef1 = coef2;
        long long new_val1 = steps[i].a;
        long long new_coef2 = coef1 - coef2 * steps[i].quotient;
        long long new_val2 = steps[i].b;
        
        coef1 = new_coef1;
        val1 = new_val1;
        coef2 = new_coef2;
        val2 = new_val2;
        
        cout << "1 = (" << coef1 << ") x " << val1 << " + (" << coef2 << ") x " << val2 << endl;
    }
    
    cout << "\n=== Step 3: Extract the Answer ===" << endl;
    cout << "\nFinal equation: 1 = (" << coef1 << ") x " << val1 << " + (" << coef2 << ") x " << val2 << endl;
    
    // Determine which coefficient goes with original_a
    long long inverse;
    if (val1 == original_a) {
        inverse = coef1;
    } else {
        inverse = coef2;
    }
    
    cout << "The coefficient of " << original_a << " is " << inverse << endl;
    
    // Make it positive if needed
    long long final_inverse = ((inverse % original_m) + original_m) % original_m;
    if (inverse < 0) {
        cout << "\nConverting to positive: " << inverse << " + " << original_m << " = " << final_inverse << endl;
    }
    
    cout << "\n=== Answer ===" << endl;
    cout << "The multiplicative inverse of " << original_a << " mod " << original_m << " is: " << final_inverse << endl;
    
    // Verification
    cout << "\n=== Verification ===" << endl;
    long long reduced_a = original_a % original_m;
    cout << original_a << " mod " << original_m << " = " << reduced_a << endl;
    
    long long product = (reduced_a * final_inverse) % original_m;
    cout << reduced_a << " x " << final_inverse << " mod " << original_m << " = " << product << endl;
    
    if (product == 1) {
        cout << "\nVerified!" << endl;
    } else {
        cout << "\nSomething went wrong..." << endl;
    }
    
    return 0;
}