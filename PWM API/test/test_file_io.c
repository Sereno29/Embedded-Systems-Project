/**
 * @file test_file_io.c
 * @brief Tests for the basic file IO under the hood.
 * @author Travis Lane
 * @version
 * @date 2014-12-22
 */

#include <bbb_pwm_internal.h>

#include "test_macros.h"

#include <assert.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

void test_open_and_close();

void test_can_write();
void test_can_read();

void test_rw_uint32();
void test_rw_int8();

void test_invalid_r_uint32();
void test_invalid_r_int8();

void test_invalid_w_uint32();
void test_invalid_w_int8();

FILE *open_testfile(const char *name, const char *mode);
void close_testfile(FILE *file);

struct test_file *test_file_new(const char *name, const char *mode);
void test_file_delete(struct test_file **tfp);

struct test_file {
  FILE 				 *tf_fp;
  char   				tf_path[1024];
  const char 	 *tf_mode;
};

int
main()
{
  test_open_and_close();

  test_can_read();
  test_can_write();

  test_rw_int8();
  test_rw_uint32();

  test_invalid_r_uint32();
  test_invalid_r_int8();

  test_invalid_w_uint32();
  test_invalid_w_int8();
}

/**
 * @brief Test if open and close work properly.
 */
void
test_open_and_close()
{
  FILE *file;
  char path[1024];

  if(getcwd(path, sizeof(path)) == NULL) {
    fprintf(stderr, "Error getting cwd!\n");
    exit(errno);
  }

  expect((strlen(path) + strlen("/test_open_and_close.txt")) < sizeof(path));
  strcat(path, "/test_open_and_close.txt");

  fprintf(stderr, "File: %s\n", path);
  file = file_open_and_claim(path, "w+");
  expect(file != NULL);

  file_close_and_unclaim(file);
}

/**
 * @brief Test if file can write passes and fails when expected.
 */
void
test_can_write()
{
  struct test_file *tf;

  tf = test_file_new("/can_write_w.txt", "w");
  expect(file_can_write(tf->tf_fp));
  test_file_delete(&tf);

  tf = test_file_new("/can_write_w+.txt", "w+");
  expect(file_can_write(tf->tf_fp));
  test_file_delete(&tf);

  tf = test_file_new("/can_write_r.txt", "r");
  expect(!file_can_write(tf->tf_fp));
  test_file_delete(&tf);
}

/**
 * @brief Test if file can read passes and fails when expected.
 */
void
test_can_read()
{
  struct test_file *tf;

  tf = test_file_new("/can_read_r.txt", "r");
  expect(file_can_read(tf->tf_fp));
  test_file_delete(&tf);

  tf = test_file_new("/can_read_w+.txt", "w+");
  expect(file_can_read(tf->tf_fp));
  test_file_delete(&tf);

  tf = test_file_new("/can_read_w.txt", "w");
  expect(!file_can_read(tf->tf_fp));
  test_file_delete(&tf);
}

/**
 * @brief Test valid read/writes of an uint32
 */
void
test_rw_uint32()
{
  uint32_t data = 0;
  struct test_file *tf;
  tf = test_file_new("/rw_uint32.txt", "w+");

  file_write_uint32(tf->tf_fp, UINT32_MAX);
  file_read_uint32(tf->tf_fp, &data);
  fprintf(stderr, "Written: %d Read: %d\n", UINT32_MAX, data);
  expect(data == UINT32_MAX);

  file_write_uint32(tf->tf_fp, 0);
  file_read_uint32(tf->tf_fp, &data);
  fprintf(stderr, "Written: %d Read: %d\n", 0, data);
  expect(data == 0);

  test_file_delete(&tf);
}

/**
 * @brief Test valid read/writes of an int8
 */
void
test_rw_int8()
{
  int8_t data = 0;
  struct test_file *tf;
  tf = test_file_new("/rw_int8.txt", "w+");

  file_write_int8(tf->tf_fp, INT8_MIN);
  file_read_int8(tf->tf_fp, &data);
  expect(data == INT8_MIN);

  file_write_int8(tf->tf_fp, INT8_MAX);
  file_read_int8(tf->tf_fp, &data);
  expect(data == INT8_MAX);

  file_write_int8(tf->tf_fp, 0);
  file_read_int8(tf->tf_fp, &data);
  expect(data == 0);

  test_file_delete(&tf);
}

/**
 * @brief Test invalid reads of a uint32
 */
void
test_invalid_r_uint32()
{
  uint32_t data = 0;
  struct test_file *tf;
  tf = test_file_new("/ir_uint32.txt", "w+");

  // Test NULL ptrs.
  expect(file_read_uint32(NULL, &data) != BPRC_OK);
  expect(file_read_uint32(tf->tf_fp, NULL) != BPRC_OK);

  // Test read from empty file.
  expect(file_read_uint32(tf->tf_fp, &data) != BPRC_OK);

  test_file_delete(&tf);
}

/**
 * @brief Test invalid reads of a int8
 */
void
test_invalid_r_int8()
{
  int8_t data = 0;
  struct test_file *tf;
  tf = test_file_new("/ir_int8.txt", "w+");

  // Test NULL ptrs.
  expect(file_read_int8(NULL, &data) != BPRC_OK);
  expect(file_read_int8(tf->tf_fp, NULL) != BPRC_OK);

  // Test read from empty file.
  expect(file_read_int8(tf->tf_fp, &data) != BPRC_OK);

  test_file_delete(&tf);
}

/**
 * @brief Test invalid writes of a uint32
 */
void
test_invalid_w_uint32()
{
  struct test_file *tf;
  tf = test_file_new("/iw_uint32.txt", "r");

  // Test NULL ptr.
  expect(file_write_uint32(NULL, 0) != BPRC_OK);

  // Test write to read only file.
  expect(file_write_uint32(tf->tf_fp, 0) != BPRC_OK);

  test_file_delete(&tf);
}

/**
 * @brief Test invalid writes of an int8
 */
void
test_invalid_w_int8()
{
  struct test_file *tf;
  tf = test_file_new("/iw_int8.txt", "r");

  // Test NULL ptr.
  expect(file_write_int8(NULL, 0) != BPRC_OK);

  // Test write to read only file.
  expect(file_write_int8(tf->tf_fp, 0) != BPRC_OK);

  test_file_delete(&tf);
}

/**
 * @brief Creates a new test file and opens it.
 *
 * @param name The name of the file to create.
 * @param mode The mode to create the file in.
 *
 * @return NULL on failure or a valid pointer.
 */
struct test_file *
test_file_new(const char *name, const char *mode)
{
  int fd;
  struct test_file *tf;

  assert(name != NULL);
  assert(mode != NULL);

  tf = malloc(sizeof(struct test_file));
  assert(tf != NULL);

  if(getcwd(tf->tf_path, sizeof(tf->tf_path)) == NULL) {
    fprintf(stderr, "Error getting cwd!\n");
    exit(errno);
  }

  expect((strlen(tf->tf_path) + strlen(name)) < sizeof(tf->tf_path));
  strcat(tf->tf_path, name);

  tf->tf_mode = mode;

  // Touch the file.
  fd = open(tf->tf_path,
            O_WRONLY | O_CREAT | O_NOCTTY | O_NONBLOCK, 0666);
  expect(fd > 0);
  close(fd);

  printf("File: %s Mode: %s\n", tf->tf_path, tf->tf_mode);

  // Actually open the file.
  tf->tf_fp = fopen(tf->tf_path, tf->tf_mode);
  assert(tf->tf_fp != NULL);

  return tf;
}

/**
 * @brief Deletes a test file.
 *
 * @param tfp A pinter to a test file.
 */
void
test_file_delete(struct test_file **tfp)
{
  struct test_file *tf;

  assert(tfp != NULL);
  tf = *tfp;

  if(tf->tf_fp != NULL) {
    fclose(tf->tf_fp);
    tf->tf_fp = NULL;
  }

  remove(tf->tf_path);

  free(tf);
  *tfp = NULL;
}
