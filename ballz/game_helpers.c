float calcSquareSide(float dispWidth) {
    return dispWidth / 7.8;
}

float calcSquareXi(float i, float l) {
    return i * 1.1*l + 0.1*l;
}

float calcSquareYi(float j, float l) {
    return j * 1.1*l;
}

float calcSquareXf(float i, float l) {
    return (i+1) * 1.1*l;
}

float calcSquareYf(float j, float l) {
    return j * 1.1*l + l;
}