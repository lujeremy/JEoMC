int main(){
    string j;
    string k;
    string l;
    string m;

    j = "FOO";
    k = "foo";
    l = "FoO";
    m = "foo";

    if (j == k) {
      print(1);
    }

    if (j == l) {
      print(2);
    }

    if (j == m) {
      print(3);
    }

    if (k == l) {
      print(4);
    }

    if (k == m) {
      print(5);
    }

    return 0;
}
