fun fib(n) {
  if (n <= 1) return n;
  var n2 = fib(n - 2);
  var n1 = fib(n - 1);

  return n2+n1;
}

// fun str (){
//   for (var i = 0; i < 20; i = i+1){
//     print i;
//   }
// }


// print fib(19);
// str();

for (var i = 0; i < 20; i = i + 1) {
  // fib(i);
  print fib(i);
  // print "  ";
  // print i;
}