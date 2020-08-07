#include <vector>
#include <iostream>



template < typename array_type , typename return_type , typename size_type >
return_type const func (  array_type* arr  , size_type length) {
    return_type sum = 0;
    size_type ln = (length/sizeof(int));
    while ( ln-- ) {
        sum += (*arr);
        arr++;
    }
    return ( sum );
}

int main ( void ) {
    int arr[5] =  { 1,1,1,1,1 };
    int x = func <int, double , int> (arr, sizeof(arr));
    std::cout << "\nThe sum is:" << x << std::endl;
    return 0;
}

