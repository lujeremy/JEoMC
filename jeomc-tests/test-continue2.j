int main(){
    bool b;
    b = true;
    while(b) {
        print(1);
        b = false;
        if (true) {
        	continue;
        }
        print(2);
    }
    print(3);
    return 0;
}