/*
 * A Factory design pattern.
 */

#include <stdio.h>
#include <stdlib.h>

#include "factory.h"
#include "logger.h"

character_factory_t *new_factory(character_t *(*factory_method)(void))
{
    character_factory_t *factory;

    factory = (character_factory_t *) malloc(sizeof(character_factory_t));
    factory->factory_method = factory_method;

    return factory;
}

character_t *new_citizen(void)
{
    character_t *citizen;

    citizen = (character_t *) malloc(sizeof(character_t));
    citizen->operation = operation_citizen;

    return citizen;
}

character_t *new_spy(void)
{
    character_t *spy;

    spy = (character_t *) malloc(sizeof(character_t));
    spy->operation = operation_spy;

    return spy;
}

character_t *new_case_officer(void)
{
    character_t *case_officer;

    case_officer = (character_t *) malloc(sizeof(character_t));
    case_officer->operation = operation_case_officer;

    return case_officer;
}

character_t *new_counterintelligence_officer(void)
{
    character_t *counterintelligence_officer;

    counterintelligence_officer = (character_t *) malloc(sizeof(character_t));
    counterintelligence_officer->operation = operation_counterintelligence_officer;

    return counterintelligence_officer;
}

//#include "../state/character.h" // redéfinit character_t :(
void operation_citizen(character_t *citizen)
{
    printf(">> Operation of a citizen...\n");

	/* ----Testing stuff----
	state_t *resting_at_home = new_state(1, rest_at_home);
	state_t *going_to_company = new_state(2, go_to_company);
	state_t *working = new_state(3, work);
	state_t *going_to_supermarket = new_state(4, go_to_supermarket);
	state_t *doing_some_shopping = new_state(5, do_some_shopping);
	state_t *going_back_home = new_state(6, go_back_home);
	citizen = new_character(resting_at_home,
	                        going_to_company,
	                        working,
	                        going_back_home,
	                        going_to_supermarket,
	                        doing_some_shopping);


	 --------------------- */
}

void operation_spy(character_t *spy)
{
    printf(">> Operation of a spy...\n");
}

void operation_case_officer(character_t *case_officer)
{
    printf(">> Operation of a case officer...\n");
}

void operation_counterintelligence_officer(character_t *counterintelligence_officer)
{
    printf(">> Operation of a counterintelligence_officer...\n");
}
