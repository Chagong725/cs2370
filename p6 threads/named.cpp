#include <iostream>
#include <fstream>
#include <vector>
#include <thread>
#include <mutex>

using namespace std;

const int MAX_NUM = 1000000;
const int NUM_THREADS = 4;

int primeCounter = 0;
int sharedCounter = 2;
vector<int> threadPrimeCount(NUM_THREADS, 0);

ofstream primeFile("primes.dat");
mutex counterLock, fileLock;

bool isPrime(int number)
{
    if (number <= 1) return false;
    if (number == 2) return true;

    for (int i = 2; i <= number / 2; i++)
    {
        if (number % i == 0)
        {
            return false;
        }
    }
    return true;
}

void findPrimes(int threadNum)
{
    while (true)
    {
        counterLock.lock();
        int candidate = sharedCounter;
        sharedCounter++;
        counterLock.unlock();

        if (candidate > MAX_NUM) break;
        if (isPrime(candidate))
        {
            fileLock.lock();
            primeFile << candidate << endl;
            primeCounter++;
            threadPrimeCount[threadNum]++;
            fileLock.unlock();
        }
    }
}

int main()
{
    vector<thread> threads;
    for (int i = 0; i < NUM_THREADS; i++)
    {
        threads.push_back(thread(findPrimes, i));
    }

    for (auto &t : threads)
    {
        t.join();
    }

    cout << "Total primes found: " << primeCounter << endl;

    for (int i = 0; i < NUM_THREADS; i++)
    {
        cout << "Thread " << i << " found " << threadPrimeCount[i] << " primes" << endl;
    }

    primeFile.close();
    return 0;
}
