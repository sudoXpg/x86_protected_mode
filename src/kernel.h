#ifndef KERNEL_H
#define KERNEL_H

#define AeOS_LOGO \
"                                 .-'''-.       \n" \
"                                '   _    \\      \n" \
"               __.....__      /   /` '.   \\     \n" \
"           .-''         '.   .   |     \\  '     \n" \
"          /     .-''\"'-.  `. |   '      |  '    \n" \
"    __   /     /________\\   \\    \\     / /     \n" \
" .:--.'. |                  | `.   ` ..' / _    \n" \
"/ |   \\ |\\    .-------------'    '-...-'`. ' |   \n" \
"`\" __ | | \\    '-.____...---.           .   | / \n" \
" .'.''| |  `.             .'          .'.'| |// \n" \
"/ /   | |_   `''-...... -'          .'.'.-'  /  \n" \
"\\ \\._,\\ '/                          .'   \\_.'   \n" \
" `--'  `\"                                       \n"

#define AeOS_MAX_PATH 108 // pathlength

#define VGA_WIDTH 80
#define VGA_HEIGHT 100

void print(const char* str);
void kernel_main();
void panic(const char *msg);



#endif