void tierra (float x, float y, float z) {
	glPushMatrix();
	//Aplicacion de texturas
	//...
	//Transformaciones
	glRotate(theta, 0, 1, 0);
	glTranslate(x, y, z);
	//...
	glPopMatrix();
}

