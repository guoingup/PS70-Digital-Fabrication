void printArray ( const int [][ 3 ] ); // prototype
const int rows = 2;
const int columns = 3;
String array1[ rows ][ columns ] = { { "Korea", "Boo", "Third" }, { "China", "June 5, 2020", "Hehe" } };
int array2[ rows ][ columns ] = { 1, 2, 3, 4, 5 };
int array3[ rows ][ columns ] = { { 1, 2 }, { 4 } };

void setup () {
  
   Serial.begin(9600);
   Serial.print ("Values in array1 by row are: ") ;
   Serial.print ("\r" ) ;
   printArray(array1) ;
}
void loop () {
   
//   Serial.print ("Values in array2 by row are: ") ;
//   Serial.print ("\r" ) ;
//   printArray(array2) ;
//   Serial.print ("Values in array3 by row are: ") ;
//   Serial.print ("\r" ) ;
//   printArray(array3) ;
}

// output array with two rows and three columns

void printArray( const String a[][ columns ] ) {
   // loop through array's rows
   for ( int i = 0; i < rows; ++i ) {
      // loop through columns of current row
      for ( int j = 0; j < columns; ++j )
      Serial.print (a[ i ][ j ] );
      Serial.print ("\r"); // start new line of output
   } 
// end outer for
} 

// end function printArray
