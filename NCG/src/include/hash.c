/*  NCG written in 2015 by Maciej A. Czyzewski

To the extent possible under law, the author has dedicated all copyright
and related and neighboring rights to this software to the public domain
worldwide. This software is distributed without any warranty.

See <http://creativecommons.org/publicdomain/zero/1.0/>.  */

#define GET_32_INT(n, b, i)                           \
{                                                     \
  (n) = ( (unsigned long) (b)[(i)    ]       )        \
      | ( (unsigned long) (b)[(i) + 1] << 8  )        \
      | ( (unsigned long) (b)[(i) + 2] << 16 )        \
      | ( (unsigned long) (b)[(i) + 3] << 24 );       \
}

#define PUT_32_INT(n, b, i)                           \
{                                                     \
  (b)[(i)    ] = (unsigned char) ( (n)       );       \
  (b)[(i) + 1] = (unsigned char) ( (n) >> 8  );       \
  (b)[(i) + 2] = (unsigned char) ( (n) >> 16 );       \
  (b)[(i) + 3] = (unsigned char) ( (n) >> 24 );       \
}

void ncg(const uint8_t *initial_message, size_t initial_length,
               uint8_t *result, size_t result_length) {
  // Cleaning state matrix
  reset();

  // Declaration of variables
  size_t length, offset;

  // Declaration of message
  uint8_t *message = NULL, *buffer = NULL;

  // Declaration of message chunk
  uint32_t chunk;

  // Calculate new length
  for (length = initial_length;
       length % 4 != 0; length++);

  // Prepare message
  message = (uint8_t*) malloc(length * 8);

  // Copy block of memory
  memcpy(message, initial_message, initial_length);

  // Complement to the full blocks
  if (length - initial_length > 0) {
    // Append "1" bit
    message[initial_length] = 0x80;

    // Append "0" bits
    for (offset = initial_length + 1; offset < length; offset++)
      message[offset] = 0;
  }

  // Append the len in bits at the end of the buffer
  PUT_32_INT(initial_length * 8, message + length, 0);

  // Initial_len >> 29 == initial_len * 8 >> 32, but avoids overflow
  PUT_32_INT(initial_length >> 29, message + length + 4, 0);

  // Process the message in successive 32-bit chunks
  for (int i = 0; i < length; i += 4) {
    // Get little endian
    GET_32_INT(chunk, message + i, 0);

    // Push to NCG structure
    push(chunk);
  }

  // Releasing memory
  free(message);

  // Allocate memory for result
  buffer = (uint8_t*) malloc(result_length * 8);

  // Process the result in successive 32-bit chunks
  for (int i = 0; i < result_length / 4 + 1; i++)
    PUT_32_INT(pull(), buffer, i * 4);

  // Save it on the pointer
  for (int i = 0; i < result_length; i++)
    result[i] = buffer[i];

  // Releasing memory
  free(buffer);
}