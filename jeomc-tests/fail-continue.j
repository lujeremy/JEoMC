int main(){
    int j;
    int i;
    j = 0;

    for (i = 0; i < 5; i = i + 1){
    if (i == 3) {
      i = i+1;
      continue;
      i = 2;
    }
        j = j+i;
    }
    print(j);
    return 0;
}