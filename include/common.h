/*
 * The License to Kill Project
 *
 * Copyright (C) 1995-2022 Alain Lebret <alain.lebret [at] ensicaen [dot] fr>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#ifndef COMMON_H
#define COMMON_H

/**
 * \file common.h
 *
 * Defines some constants and paths to interprocess mechanisms.
 */

/*
 * ---------------------------------------------------------------------------
 * Constants associated with the city map
  * ---------------------------------------------------------------------------
 */
#define MAX_COLUMNS 7 /*!< Maximum number of columns for the city map. */
#define MAX_ROWS    7 /*!< Maximum number of rows for the city map. */

/*
 * ---------------------------------------------------------------------------
 * Constants associated with the content of messages.
 * ---------------------------------------------------------------------------
 */

/** Maximum length of a message and maximum of messages */
#define MAX_LENGTH_OF_MESSAGE 128
#define MAX_NB_MSG 13

/** Name of the enemy county message mqueue */
#define ENEMY_MQUEUE "/spy_msg_queue"

#define FAKE_MESSAGE  "FAKE"
#define EMPTY         "EMPTY"
#define STOLEN        "STOLEN"
#define NOT_FOUND     "NOT_FOUND"

#define NB_OF_NORMAL_CITIZEN 128

#endif /* COMMON_H */
