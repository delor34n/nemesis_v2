void Keyboard(unsigned char key, int x, int y){
	switch (key) {
		//Aumenta velocidad
		case 'j':
			if ( speed-0.5 > 0 )
				speed-=0.05;
			else
				speed = 0.001;
			break;
		//Disminuye la velocidad
		case 'k':
			speed+=0.05;
		break;
	}
}
