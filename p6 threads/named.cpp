#include <iostream>
#include <fstream>
#include <vector>
#include <thread>
#include <mutex>

using namespace std;

const int MAX_NUM = 1000000;
const int NUM_THREADS = 4;

int primeCounter = 0;
int sharedCounter = 2; //2부터 시작
vector<int> threadPrimeCount(NUM_THREADS, 0); // 스레드가 찾은 prime mun갯수

ofstream primeFile("primes.dat");
mutex counterLock, fileLock;

bool isPrime(int number)
{
    if (number <= 1) return false;
    if (number == 2) return true;

    for (int i = 2; i <= number / 2; i++)
    {
        if (number % i == 0) // remainder확인, quotient는 x
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
        counterLock.lock(); //뮤텍스 잠금으로 다른 스레드가 sharedCounter 못 바꿈
        int candidate = sharedCounter;
        sharedCounter++;
        counterLock.unlock();

        if (candidate > MAX_NUM) break; 
        if (isPrime(candidate))
        {
            fileLock.lock();
            primeFile << candidate << endl;
            primeCounter++;
            threadPrimeCount[threadNum]++; //이 스레드가 찾은 소수 갯수
            fileLock.unlock();
        }
    }
}

int main()
{
    vector<thread> threads;
    for (int i = 0; i < NUM_THREADS; i++)
    {
        threads.push_back(thread(findPrimes, i)); //fP함수 실행 스레드 벡터의 추가, i는 번호
    }

    for (auto &t : threads) //오토로 t 타입, &로 스레트 참조 
    {
        t.join(); //조인으로 현재 스레드 블록
    }

    cout << "Total primes found: " << primeCounter << endl;

    for (int i = 0; i < NUM_THREADS; i++) //각 스레드 번호와 해당 스레드가 찾은 소수 출력
    {
        cout << "Thread " << i << " found " << threadPrimeCount[i] << " primes" << endl;
    }

    primeFile.close();
    return 0;
}
