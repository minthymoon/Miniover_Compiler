// 1. Function with parameters and explicit return type
fn calculate_discount(price float, count int) -> float {
    // 2. Arithmetic with implicit conversion (float * int -> float)
    let total float = price * count; 
    
    // 3. Built-in print (ad-hoc polymorphism for float)
    print(total);
    
    // 4. Comparison and logic (bool return)
    if (total > 100.0) {
        let discount float = total * 0.1;
        print(discount);
        return total - discount;
    } else {
        print(0.0);
        return total;
    }
}

fn main() -> int {
    // 5. Local variable declarations and literals
    let items int = 15;
    let rate float = 9.5;
    
    // 6. Function call and assignment
    let final_price float = calculate_discount(rate, items);
    
    // 7. While loop for control flow testing
    let counter int = 0;
    while (counter < 3) {
        print(counter);
        counter = counter + 1;
    }

    print(final_price);
    return 0;
}