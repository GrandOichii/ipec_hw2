#include <iostream>
#include <ctime>
#include <vector>
#include <thread>

int N, M, K, T;

class barrier {
public:
    barrier(int count)
     : thread_count(count)
     , counter(0)
     , waiting(0)
    {}

    void wait()
    {
        std::unique_lock<std::mutex> lk(m);
        ++counter;
        ++waiting;
        cv.wait(lk, [&]{return counter >= thread_count;});
        cv.notify_one();
        --waiting;
        if(waiting == 0)
        {
            counter = 0;
        }
        lk.unlock();
    }

 private:
    std::mutex m;
    std::condition_variable cv;
    int counter;
    int waiting;
    int thread_count;
};


bool** create() {
    bool** result = new bool*[N];
    for (int i = 0; i < N; i++) {
        result[i] = new bool[M];
        for (int j = 0; j < M; j++) {
            result[i][j] = rand() % 2;
        }
    }
    return result;
}

void destroy(bool** space) {
    for (int i = 0; i < M; i++)
        delete[] space[i];
    delete[] space;
}

void print(bool** space) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++)
            std::cout << (space[i][j] ? "*" : ".") << " ";
        std::cout << std::endl;
    }
}

int get_alive_neighbors(bool** space, int i, int j) {
    int result = 0;
    for (int ii = -1; ii <= 1; ii++) {
        for (int jj = -1; jj <= 1; jj++) {
            if (ii == 0 && jj == 0) continue;
            int posI = i + ii;
            int posJ = j + jj;
            if (posI < 0 || posJ < 0 || posI >= N || posJ >= M) continue;
            result += space[posI][posJ];
        }
    }

    return result;
}

bool** advance(bool** space) {
    bool** result = create();

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            int n = get_alive_neighbors(space, i, j);
            result[i][j] = space[i][j];
            if (space[i][j]) {
                if (n < 2 || n > 3) result[i][j] = false;
            } else if (n == 3) {
                result[i][j] = true;
            }
        }
    }
    return result;
}

int main(int argc, char* argv[]) {
    if (argc != 5) {
        std::cerr << "Incorrent number of arguments" << std::endl;
        return 1;
    }
    // srand(time(0));
    srand(0);

    N = atoi(argv[1]);
    M = atoi(argv[2]);
    K = atoi(argv[3]);
    T = atoi(argv[4]);

    if (T > N) T = N;

    bool** space = create();
    bool** temp = create();

    std::mutex m;
    // std::cout << "N: " << N << std::endl;
    // std::cout << "M: " << M << std::endl;
    // std::cout << "K: " << K << std::endl;
    // std::cout << "T: " << T << std::endl;
    barrier populate_b(T+1);
    barrier replace_b(T+1);

    auto main_thread = [&]() {
        for (int i = 0; i < K; i++) {
            // m.lock();
            // std::cout << "( EPOCH " << i << " )" << std::endl;
            // m.unlock();

            temp = create();

            // m.lock();
            // std::cout << "CREATED NEW SPACE, POPPULATING..." << std::endl;
            // m.unlock();

            replace_b.wait();
            populate_b.wait();

            // m.lock();
            // std::cout << "SPACE POPULATED, REPLACING" << std::endl;
            // m.unlock();

            destroy(space);
            space = temp;

            // m.lock();
            // std::cout << std::endl;
            // print(space);
            // std::cout << std::endl;
            // m.unlock();

        }
    };

    auto t_fun = [&](int sI, int sJ, int eI, int eJ) {
        for (int i = 0; i < K; i++) {
            replace_b.wait();

            for (int ii = sI; ii <= eI; ii++) {
                for (int jj = sJ; jj <= eJ; jj++) {
                    int n = get_alive_neighbors(space, ii, jj);
                    temp[ii][jj] = space[ii][jj];
                    if (space[ii][jj]) {
                        if (n < 2 || n > 3) temp[ii][jj] = false;
                    } else if (n == 3) {
                        temp[ii][jj] = true;
                    }
                }
            }

            // m.lock();
            // std::cout << sI << " : " << sJ << " : " << eI << " : " << eJ << std::endl;
            // m.unlock();

            populate_b.wait();
        }
    };

    std::thread threads[T+1];
    int step = N / T;
    for (int i = 0; i < T; i++) {
        int sI = i * step;
        int eI = i * step + step - 1;
        if (i == T - 1) eI = N - 1;

        threads[i] = std::thread(t_fun, sI, 0, eI, M - 1);
    }
    threads[T] = std::thread(main_thread);

    for (int i = 0; i < T+1; i++)
        threads[i].join();

    destroy(space);

    return 0;
}