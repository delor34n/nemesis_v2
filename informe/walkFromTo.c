int walkFromTO ( float fromX, float fromZ, float toX, float toZ ) {
    float newDistanceX = (toX - fromX)/50;
    float newDistanceZ = (toZ - fromZ)/50;
    der = fromX + newDistanceX;
    z = fromZ + newDistanceZ;
    if ( fabs(toX-der) > 0.1 || fabs(toZ-z)> 0.1 )
        return 1;
    else
        return 0;
}
