fn addAndPrint(a int, b float) -> float {
    let result float = a + b;   // int + float → float
    print(result);
    return result;
}

fn main() -> int {
    let x int = 10;
    let y float = 3.2;
    let z = addAndPrint(x, y);   // z is float (13.2)
    if (z > 10.0) {
        print(42);
    } else {
        print(0);
    }
    return 0;
}