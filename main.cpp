#include <iostream>
#include <string>
#include <iomanip>

using namespace std;

class InvalidInput{

    private:
        string message;

    public:
        InvalidInput(string s) : message(){
            message = "Invalid input [" + s + "].\n";
        };

    void cause(){
        cout << message;
    }

};

class DeliveryOrder{

    private:
        string name;
        int month, day, year;
        string phone;
        double miles;

        static int orderCount;

    public:
        static const double taxRate, deliveryRate;
        double balance;

    DeliveryOrder(string name, int month, int day, int year, string phone, double miles){
        this->balance = 0;
        this->name = name;
        this->month = month;
        this->day = day;
        this->year = year;
        this->phone = phone;
        this->miles = miles;
        orderCount++;
    }

    ~DeliveryOrder(){
        cout << "DeliveryOrder destroyed.\n";
    }

    void printReceipt() const{
        cout << "Order Detail:" << "\n";
        cout << "\t" << name << "\n";
        cout << "\tDate: " << month << "/" << day << "/" << year << "\n";
        cout << "\tPhone: " << phone << "\n";
        cout << "\tTotal Balance: $" << balance << endl;
    }

    double getBalance() const{
        return balance * (1 + taxRate) + miles * deliveryRate;
    }

    int static getOrderCount(){
        return orderCount;
    }

    virtual double VIPdiscount()=0;

};

class BobaOrder : public DeliveryOrder {

    public:
        string shopName;
        int drinksCount;
    
    BobaOrder(string name, int month, int day, int year, string phone, double miles, string shopName):DeliveryOrder(name, month, day, year, phone, miles){
        this->shopName = shopName;
        drinksCount = 0;
    }
    ~BobaOrder(){
        cout << "BobaOrder destroyed.\n";
    }
    
    void printReceipt() const{
        DeliveryOrder::printReceipt();
        cout << "\tOrdered Drinks: " << drinksCount << endl;
    }

    double getBalance() const{
        return DeliveryOrder::getBalance(); // pretty sure prof did not mean for us to use it this way, will find another way later
    }

    double VIPdiscount(){
        if(drinksCount >= 10){
            return 0.85;
        }else if(drinksCount  < 10 & drinksCount  >= 5){
            return 0.9;
        }else if(drinksCount  < 5 & drinksCount  >= 2){
            return 0.95;
        }else{
            return 1;
        }
    }

    void addDrink(string drink, bool t = true, int sameDrink = 1){
        
        int addOne = 0;

        if(t == true){
            addOne = 1;
        }

        if(drink == "Matcha Lemonade"){
            this-> drinksCount += sameDrink;
            DeliveryOrder::balance += (5.5 + addOne) * sameDrink;
        }else if(drink == "Brown Sugar Oolong Tea"){
            this-> drinksCount += sameDrink;
            DeliveryOrder::balance += (5 + addOne) * sameDrink;
        }else if(drink == "Lemon Green Tea"){
            this-> drinksCount += sameDrink;
            DeliveryOrder::balance += (5.25 + addOne) * sameDrink;
        }else{
            throw InvalidInput (drink);
            cout << "BOBA_INVALID_INPUT_ELSE CALLED" << endl;
        }

    }

};

class FoodOrder : public DeliveryOrder {

    public:
        string restaurantName;
        int foodCount;
    
    FoodOrder(string name, int month, int day, int year, string phone, double miles, string restaurantName):DeliveryOrder(name, month, day, year, phone, miles){
        this->restaurantName = restaurantName;
        foodCount = 0;
    }
    ~FoodOrder(){
        cout << "FoodOrder destroyed.\n";
    }
    
    void printReceipt() const{
        DeliveryOrder::printReceipt();
        cout << "\tOrdered Foods: " << foodCount << endl;
    }

    double VIPdiscount(){
        if(DeliveryOrder::balance >= 50){
            return 0.85;
        }else if(DeliveryOrder::balance < 50 & DeliveryOrder::balance >= 30){
            return 0.9;
        }else if(DeliveryOrder::balance < 30 & DeliveryOrder::balance >= 20){
            return 0.95;
        }else{
            return 1;
        }
    }

    void addFood(string name, int a = 0, bool f = false){

        double soup = 0;

        if(f == true){
            soup = 0.5;
        }        

        if(name == "Bone-in Ribeye"){
            this->foodCount++;
            DeliveryOrder::balance += 32 + (a * 1) + soup;
        }else if(name == "Rack of Lamb"){
            this->foodCount++;
            DeliveryOrder::balance += 28 + (a * 1) + soup;
        }else if(name == "Grilled Salmon"){
            this->foodCount++;
            DeliveryOrder::balance += 24 + (a * 1) + soup;
        }else if(name == "Beyond Meat Burger"){ 
            this->foodCount++;
            DeliveryOrder::balance += 22 + (a * 1) + soup;
        }else{

        }

    }

};

class Account {

    private:
        string username, status;

    public:

    Account(string username, string status){
        this->username = username;
        this->status = status;
    }
    ~Account(){
        cout << "Account removed.\n";
    }

    string getStatus() const{
        return status;
    }

};

double applyDiscount(DeliveryOrder *d, Account const &a){

        if(a.getStatus() == "Owner"){
            return 0.1*d->getBalance();
        }else if(a.getStatus() == "VIP"){
            return d->VIPdiscount()*d->getBalance();
        }else{
            return d->getBalance();
        }

    }

double const DeliveryOrder::taxRate = 0.0887 ;
double const DeliveryOrder::deliveryRate = 1.5;
int DeliveryOrder::orderCount = 0;

int main(){

    const Account Stuart ("Stuart","Owner");
    Account Kevin ("Kevin", "VIP");
    Account Bob ("Bob"," ");

    cout << "Kevin placing order.\n";

    BobaOrder kevinDrink("Kevin",3,2,2021,"123-456-0000",10.4,"Bar Pa Tea");

    try{
        kevinDrink.addDrink("Matcha Lemonade",true,1);
        kevinDrink.addDrink("Lemon Green Tea",false,1);
        kevinDrink.addDrink("Brown Sugar Oolong Tea",false,2);
        kevinDrink.addDrink("Iron Goddess",false,1);
    }catch (InvalidInput &x){
        x.cause();
        cout << "Not serving requested drinks. Drink order ignored.\n\n";
    }

    DeliveryOrder *p = &kevinDrink;
    cout.precision(5);

    kevinDrink.printReceipt();
    cout << "Balance: $" << kevinDrink.getBalance() << endl;
    cout << "Discounted Balance: $" << applyDiscount(p,Kevin) << "\n" << endl;
    cout << "Stuart placing order.\n" << endl;

    FoodOrder stuartFood("Stuart",3,2,2021,"123-456-1111",25.5,"Trauts Steak House");
    
    try{
        stuartFood.addFood("Bone-in Ribeye",2,true);
        stuartFood.addFood("Grilled Salmon",1,false);
        stuartFood.addFood("Beyond Meat Burger",3,3);
    }catch (InvalidInput &x){
        x.cause();
        cout << "Not serving requested food. Food order ignored.\n\n";
    }

    DeliveryOrder *s = &stuartFood;
    stuartFood.printReceipt();
    cout << "Balance: $" << stuartFood.getBalance() << endl;
    cout << "Discounted Balance: $" << applyDiscount(s,Stuart) << "\n" << endl;
    cout << "Bob decided to log in to his account and see whether he can afford ordering the same order as Stuart.\n";
    
    stuartFood.printReceipt();
    cout << "Balance: $" << stuartFood.getBalance() << endl;
    cout << "Discounted Balance: $" << applyDiscount(s,Bob) << endl;
    cout << "Bob upset, cancelling order :(\n\n";
    cout << "Total order place: " << DeliveryOrder::getOrderCount() << endl;
    

    return 0;
}