#include <iostream>
#include <ctime>
#include "opencv2/core.hpp"
using namespace std;
int quickPartition(ushort *_array, int _start, int _end, int povit) {
    int i = _start;
    for (int index = _start; index < _end; ++index) {
        if (_array[index] <= povit) {
            // swap (index, i).
            ushort tmp = _array[index];
            _array[index] = _array[i];
            _array[i] = tmp;
            ++i;
        }
    }
    // swap (i, _end).
    if (i != _end) {
        ushort tmp = _array[i];
        _array[i] = _array[_end];
        _array[_end] = tmp;
    }
    return i;
}

void quickSort(ushort *_array, int _start, int _end) {
    if (_start < _end) {
        int povit = _array[_end];
        int threshold = quickPartition(_array, _start, _end, povit);
        cout << "_start=" << _start << ", _end =" << _end << ", threshold = " << threshold << endl;
        quickSort(_array, _start, threshold -1);
        quickSort(_array, threshold +1, _end);
    }
}

void quickSelection(ushort *_array, int _start, int _end, int _element, int *value) {
    if (_start < _end) {
        int povit = _array[_end];
        int threshold = quickPartition(_array, _start, _end, povit);
        if (threshold == _element) {
            *value = _array[_element];
        } else if (threshold > _element) {
            quickSelection(_array, _start, threshold -1, _element, value);
        } else {
            quickSelection(_array, threshold + 1, _end, _element, value);
        }
    } else if (_start == _end) {
        *value = _array[_start];
    } else {

    }
}

int main() {

    std::srand(std::time(0)); 
    ushort array[7] = {0};
    for (int i = 0; i < 7; ++i) {
        array[i] = std::rand() / 100;
    }
    //ushort array[] = {1, 1, 1, 1, 1, 1, 1};
    int threshold = quickPartition(array, 0, 6, 4);

    cout << "threshold = " << threshold << endl;
    for (int i = 0; i < sizeof (array) / sizeof (ushort); ++i) {
        cout << array[i] << ",";
    }

    int value = 0;
    quickSelection(array, 0, 6, 2, &value);
    cout << "value = " << value << endl;

    cout << endl;
    quickSort(array, 0, 6);
    for (int i = 0; i < sizeof (array) / sizeof (ushort); ++i) {
        cout << array[i] << ",";
    }
    return 0;
}
