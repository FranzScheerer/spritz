#include <stdio.h>
#include <stddef.h>
#include <string.h>

typedef struct State_0_ {
   unsigned char s[256];
   unsigned char a,i,j,k,w,z;
} State_0;

static void initialize_state(State_0 *state, unsigned char shift) {
   for (int v = 0; v < 256; v++) {
       state->s[v] = (unsigned char) v;
       state->s[v] += shift;
   }
   state->a = state->i = state->j = 
   state->k = state->w = state->z = 3;
}

static void update(State_0 *state) {
   unsigned char t;
   unsigned char y;
   state->i += state->w;
   y = state->j + state->s[state->i];
   state->j = state->k + state->s[y];
   state->k = state->i + state->k + state->s[state->j];
   t = state->s[state->i];
   state->s[state->i] = state->s[state->j];
   state->s[state->j] = t;
}

static unsigned char output(State_0 *state) {
   const unsigned char y1 = state->z + state->k;
   const unsigned char x1 = state->i + state->s[y1];
   const unsigned char y2 = state->j + state->s[x1];
   state->z = state->s[y2];
   return state->z;
}

static void absorb_nibble(State_0 *state, const unsigned char x) {

   unsigned char t;
   if (state->a == 128) {
     for(int v = 0; v < 512; v++){ 
        update(state);
     }
     state->w += 2;
     state->a = 0;
   }
  
   t = state->s[state->a];
   state->s[state->a] = state->s[128 + x];
   state->s[128 + x] = t;
   state->a++;
}

static void absorb_byte(State_0 *state, const unsigned char b) {
   state->j += b;
   absorb_nibble(state, b & 0xf);
   absorb_nibble(state, b >> 4);
}

int main(int argn, char **argv){
 int i,c,cc;
 char pw[] = "top secret";
 State_0 sa, sb;
 initialize_state(&sa, 3);
 initialize_state(&sb, 7);
 i = 0;
 while (pw[i]){
    absorb_byte(&sa, pw[i++]);
 }
 while ((c = fgetc(stdin)) != -1){
    cc = c ^ output(&sa) ^ output(&sb);
    absorb_byte(&sb, cc);
    fprintf(stdout,"%c", cc);
 }
 for (i=0;i<32;i++){
    update(&sb);
    fprintf(stderr, "%02X", output(&sb));
 }
 fprintf(stderr, "\n");
 return 0;
}
