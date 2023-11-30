#define F_CPU 16000000UL

#define RS PORTD0
#define E PORTD1
#define lcd_port PORTD
#define lcd_data_dir DDRD

#include <avr/io.h>
#include <util/delay.h>

void lcd_initialize();
void lcd_cmd(unsigned char);
void lcd_clear();
void lcd_print(char *);
void lcd_print_pos(unsigned char row, unsigned char pos, char *str);

int main() {
	lcd_initialize();
	lcd_print("HELLO");

	while (1) {
	}
}

void lcd_initialize(void) {
	lcd_data_dir = 0xff;
	_delay_ms(15);
	lcd_cmd(0x02);
	lcd_cmd(0x28); 
	lcd_cmd(0x0c);
	lcd_cmd(0x06);
	lcd_cmd(0x01); 
	_delay_ms(2);
}

void lcd_cmd(unsigned char cmd) {
	lcd_port = (lcd_port & 0x0F) | (cmd & 0xF0);
	lcd_port &= ~(1 << RS);
	lcd_port |= (1 << E);
	_delay_us(200);
	lcd_port &= ~(1 << E);
	_delay_ms(2);

	lcd_port = (lcd_port & 0x0F) | (cmd << 4);
	lcd_port |= (1 << E);
	_delay_us(200);
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
		lcd_port = (lcd_port & 0x0F) | (str[i] & 0xF0);
		lcd_port |= (1 << RS);
		lcd_port |= (1 << E);
		_delay_us(1);
		lcd_port &= ~(1 << E);
		_delay_ms(200);
		lcd_port = (lcd_port & 0x0F) | (str[i] << 4);
		lcd_port |= (1 << E);
		_delay_us(1);
		lcd_port &= ~(1 << E);
		_delay_ms(2);
	}

}

void lcd_print_pos(unsigned char row, unsigned char pos, char *str) {
	if(row == 0 && pos < 16) {
		lcd_cmd((pos & 0x0F) | 0x80);
	} else if(row == 1 && pos < 16) {
		lcd_cmd((pos & 0x0F) | 0xC0);
	}

	lcd_print(str);
}
