#define RS PORTD0
#define E PORTD1
#define D4 PORTB2
#define D5 PORTB3
#define D6 PORTB4
#define D7 PORTB5
#define lcd_port PORTB
#define lcd_data_dir DDRB

#include <avr/io.h>
#include <util/delay.h>

void lcd_initialize();
void lcd_cmd(unsigned char);
void lcd_clear();
void lcd_print(char *);
void lcd_print_pos(unsigned char row, unsigned char pos, char *str);
void deliver_data(unsigned char);

int main() {
	DDRD |= (1 << DDD0) | (1 << DDD1);
	lcd_data_dir |= (1 << DDB2) | (1 << DDB3) | (1 << DDB4) | (1 << DDB5);
	lcd_initialize();
	lcd_print("Hello");

	while (1) {
		lcd_print("4bit LCD");
	}
}

void lcd_initialize(void) {
	_delay_ms(15);
	lcd_cmd(0x28);
	lcd_cmd(0x0c); 
	lcd_cmd(0x06);
	lcd_cmd(0x01);
	_delay_ms(2);
}

void deliver_data(unsigned char data) {
	if((data&0b00010000) == 0b00010000) {
		lcd_port |= (1 << D4);
	} else {
		lcd_port &= ~(1 << D4); 
	}
	if((data&0b00100000) == 0b00100000) {
		lcd_port |= (1 << D5);
	} else {
		lcd_port &= ~(1 << D5); 
	}
	if((data&0b01000000) == 0b01000000) {
		lcd_port |= (1 << D6);
	} else {
		lcd_port &= ~(1 << D6); 
	}
	if((data&0b10000000) == 0b10000000) {
		lcd_port |= (1 << D7);
	} else {
		lcd_port &= ~(1 << D7); 
	}
}

void lcd_cmd(unsigned char cmd) {
	deliver_data(cmd&0xF0);
	lcd_port &= ~(1 << RS);
	_delay_ms(100);
	lcd_port |= (1 << E);
	_delay_ms(100);
	lcd_port &= ~(1 << E);
	_delay_ms(2);

	deliver_data(cmd << 4);
	lcd_port &= ~(1 << RS);
	_delay_ms(100);
	lcd_port |= (1 << E);
	_delay_ms(100);
	lcd_port &= ~(1 << E);
	_delay_ms(2);
}

void lcd_clear(void) {
	lcd_cmd(0x01);
	_delay_ms(2);
	lcd_cmd(0x80);
}

void lcd_print(char *str) {
	for(int i = 0; str[i] != '\0'; i++) {
		deliver_data(str[i]&0xF0);
		lcd_port |= (1 << RS);
		_delay_ms(5);
		lcd_port |= (1 << E);
		_delay_ms(5);
		lcd_port &= ~(1 << E);
		_delay_us(200);

		deliver_data(str[i] << 4);
		lcd_port |= (1 << RS);
		_delay_ms(5);
		lcd_port |= (1 << E);
		_delay_ms(5);
		lcd_port &= ~(1 << E);
		_delay_ms(2);
	}

}

/*
void lcd_print_pos(unsigned char row, unsigned char pos, char *str) {
	if(row == 0 && pos < 16) {
		lcd_cmd((pos & 0x0F) | 0x80);
	} else if(row == 1 && pos < 16) {
		lcd_cmd((pos & 0x0F) | 0xC0);
	}

	lcd_print(str);
}
*/
