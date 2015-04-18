#include <assert.h>

#include <iostream>
#include <string>

using namespace std;

int main() {
    const size_t max_length = 32;
    string a;
    a.assign(max_length, 'a');
    string b = a;
    if (b.data() == a.data()) {
        cout << "\ta.data() == b.data()" << endl;
    } else {
        cout << "\ta.data() != b.data()" << endl;
    }
    b.resize(max_length);

    cout << "\nafter b.resize(" << max_length << "):\n";
    if (b.data() == a.data()) {
        cout << "\ta.data() == b.data()" << endl;
    } else {
        cout << "\ta.data() != b.data()" << endl;
    }
    assert(a.data() == b.data());

    size_t new_size = max_length / 2;
    b.resize(new_size);
    cout << "\nafter b.resize(" << new_size << "):\n";
    if (b.data() == a.data()) {
        cout << "\ta.data() == b.data()" << endl;
    } else {
        cout << "\ta.data() != b.data()" << endl;
    }
    assert(a.data() != b.data());

    return 0;
}

