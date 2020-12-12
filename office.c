#include "office.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include  <ctype.h>



// pass in the boss and find the first child that is not supervising
struct employee *getSupervisor(struct employee *p)
{

  if(p->n_subordinates== 0)
  {
    return p;
  }
  else
  {
    int num = p->n_subordinates;
    struct employee *children = malloc(num * sizeof(struct employee));
    int element = 0;
    for (int i = 0; i < p->n_subordinates; i++)
    {
      if(p->subordinates[i].n_subordinates >0)
      {
        for(int j = 0 ; j < p->subordinates[i].n_subordinates; j++)
        {
          struct employee *ptr = &children[j];
          struct employee *ptr2= &p->subordinates[i].subordinates[j];



          memcpy(ptr,ptr2,sizeof(struct employee));
          element +=1;
        }
      }
      else
      {
        free(children);
        return &p->subordinates[i];
      }
    }
    if (element > 0)
    {
      for (int k = 0; k< element;k++)
      {
        return getSupervisor(&children[k]);
      }
    }

  }
  return NULL;
}







struct employee *GetEmployee(struct employee *officeSupervisor, struct employee *e)
{
    struct employee *ptr = NULL;
    if(officeSupervisor !=NULL && e != NULL)
    {
      if ((strcmp(officeSupervisor->name,e->name)==0) )
      {
        ptr = officeSupervisor;
      }
      else
      {

        for(int i = 0; i<officeSupervisor->n_subordinates;i++)
        {

          if (strcmp(officeSupervisor->subordinates[i].name,e->name)==0)
          {
            ptr = &officeSupervisor->subordinates[i];
          }
          else
          {
            if((officeSupervisor->subordinates[i].n_subordinates != 0))
            {
              ptr = GetEmployee(&officeSupervisor->subordinates[i], e);
            }

          }
        }
      }
    }
    else{
      printf("something is NULL\n");
    }



    return ptr;

}







/**
 * Places an employee within the office, if the supervisor field is NULL
 *  it is assumed the employee will be placed under the next employee that is
 * not superivising any other employee (top-down, left-to-right).
 *
 * If the supervisor is not NULL, it will be added to the supervisor's subordinates list
 *  of employees (make sure the supervisor exists in hierarchy).
 * if the office or employee are null, the function not do anything.
 */
 void office_employee_place(struct office* off, struct employee* supervisor,
  struct employee* emp)
  {

    if(off== NULL || emp == NULL)
    {
      return;
    }
    else
    {
      if(off->department_head == NULL)
      {
        //add first emp witth new office
        off->department_head = malloc(sizeof(struct employee)*100);
        off->department_head->name = malloc(sizeof(char)*10);
        strcpy(off->department_head->name,emp->name);
        off->department_head->n_subordinates = emp->n_subordinates;
      }

      else if (supervisor != NULL)
      {



        struct employee *e = GetEmployee(off->department_head,supervisor);

        int index = e->n_subordinates;
        emp->supervisor = e;


        // printf("address of 2 in place: %zu\n", e);


        if(strcmp(e->name,off->department_head->name)==0)
        {

          // printf("office sub hello wf: %zu\n",off->department_head->n_subordinates);
          if(index == 0)
          {

            off->department_head->subordinates = malloc(sizeof(struct employee)*100);
            off->department_head->subordinates[0] = *emp;
            off->department_head->subordinates[0].name = malloc(sizeof(char)*10);
            memmove(off->department_head->subordinates[0].name,emp->name,strlen(emp->name)+1);



          }
          else
          {

            e->subordinates = realloc(off->department_head->subordinates,sizeof(struct employee)*100);
            e->subordinates[index] = *emp;
            e->subordinates[index].name = malloc(sizeof(char)*10);
            memmove(off->department_head->subordinates[index].name,emp->name,strlen(emp->name)+1);

          }
        }


        else
        {
          if(index == 0)
          {
            e->subordinates = malloc(sizeof(struct employee));
            e->subordinates[index] = *emp;
            e->subordinates[index].name = malloc(sizeof(char)*10);
            strcpy(e->subordinates[index].name,emp->name);
          }
          else
          {
            e->subordinates = realloc(e->subordinates,sizeof(struct employee)*10);
            e->subordinates[index] = *emp;
            e->subordinates[index].name = malloc(sizeof(char)*10);
            strcpy(e->subordinates[index].name,emp->name);
          }

        }
        e->n_subordinates +=1;



      }

      else
      {


        struct employee *e = getSupervisor(off->department_head);


        struct employee *p = GetEmployee(off->department_head,e);

        emp->supervisor = p;

        p->subordinates = malloc(sizeof(struct employee)*100);
        p->subordinates[0] = *emp;

        p->subordinates[0].name = malloc(sizeof(char)*10);
        strcpy(p->subordinates[0].name,emp->name);

        p->subordinates[0].n_subordinates = emp->n_subordinates;
        p->n_subordinates+=1;


      }


      // free(newEmp);
    }
  }



// * Fires an employee, removing from the office
// * If employee is null, nothing should occur
// * If the employee does not supervise anyone, they will just be removed
// * If the employee is supervising other employees, the first member of that
// *  team will replace him.
// */
// void office_fire_employee(struct employee* employee);

void office_fire_employee(struct employee* employee)
{
  if(employee == NULL)
  {
    return;
  }
  else if (employee->n_subordinates == 0)
  {

    int position = 0;
    for(int i = 0; i < employee->supervisor->n_subordinates;i++)
    {
      if(strcmp(employee->supervisor->subordinates[i].name, employee->name)==0)
      {
        position = i;
      }
    }
    struct employee *ptr = &employee->supervisor->subordinates[position];
    free(employee->supervisor->subordinates[position].name);
    employee->supervisor->subordinates[position].name = NULL;
    free(ptr);


    for(int j = position-1;j<employee->supervisor->n_subordinates-1;j++ )
    {
      employee->supervisor->subordinates[j] = employee->supervisor->subordinates[j + 1];

    }
    employee->supervisor->n_subordinates = employee->supervisor->n_subordinates-1;
  }

  else
  {

    //BOSS REMOVE ME
      // employee->supervisor->subordinates = realloc(employee->supervisor->subordinates,sizeof(struct employee)*10);
      // printf("my name %s and boss name %s\n", employee->name, employee->supervisor->name);

      int position = 0;
      for(int i = 0; i < employee->supervisor->n_subordinates;i++)
      {
        if(strcmp(employee->supervisor->subordinates[i].name,employee->name)==0)
        {
          position = i;

        }
      }



      //BOSS ADD ME
      struct employee *newSup = &employee->subordinates[0];
      strcpy(employee->supervisor->subordinates[position].name,newSup->name);



      //Supervisor swapping
      for(int i = 1; i < employee->n_subordinates;i++)
      {
        employee->subordinates[i].supervisor = newSup;
      }

      //change children n_subordinates
      newSup->n_subordinates = employee->n_subordinates-1;

      //swapping the children



    }



  // }
}


struct employee* getbyName(struct employee *e, const char *name)
{
  struct employee *ptr = NULL;

  if ((strcmp(e->name,name)==0))
  {
    ptr = e;
  }

  else
  {
    for(int i = 0; i<e->n_subordinates;i++)
    {
      if (strcmp(e->subordinates[i].name,name)==0)
      {
        ptr = &e->subordinates[i];
      }
      else
      {
        if(e->subordinates[i].n_subordinates != 0)
        {
          ptr = getbyName(&e->subordinates[i],name);
        }

      }
    }
  }

  return ptr;
}





struct employee* office_get_first_employee_with_name(struct office* office,
  const char* name)
  {

    if(office == NULL ||name == NULL)
    {
      return NULL;
    }
    else
    {
      struct employee *e = getbyName(office->department_head,name);
      if(e==NULL)
      {
        return NULL;
      }
      return e;
    }

  }





  //
void getbyNameList(struct employee *e, const char *name, size_t* n_employees,struct employee **list)
  {
    //MALLOCING EACH TIME


    struct employee *ptr = NULL;
    //VALUE disappear when recurrsion havppen as well as memory leak
    if(e ==NULL || name == NULL)
    {
      return ;
    }

    if ((strcmp(e->name,name)==0))
    {

      ptr = e;

      *list = realloc(*list, sizeof(struct employee)*2);
      (*list)[(*n_employees)] = *ptr;
      (*n_employees) +=1;
    }

    for(int i = 0; i<e->n_subordinates;i++)
    {
      if (strcmp(e->subordinates[i].name,name)==0 &&((e->subordinates[i].n_subordinates == 0)))
      {

        // printf("address: %zu\n",e->subordinates[i]);


        *list = realloc(*list, sizeof(struct employee)*10);
        ptr = &e->subordinates[i];
        (*list)[(*n_employees)] = *ptr;

        (*n_employees) +=1;

      }

      else
      {
        if(e->subordinates[i].n_subordinates != 0)
        {

          getbyNameList(&e->subordinates[i],name,n_employees,list);

        }

      }
    }

  }

void getListCount(struct employee *e, char *name,size_t* n_employees )
{

  if ((strcmp(e->name,name)==0))
  {
    (*n_employees) +=1;
  }

  for(int i = 0; i<e->n_subordinates;i++)
  {
    if (strcmp(e->subordinates[i].name,name)==0 &&((e->subordinates[i].n_subordinates == 0)))
    {
      (*n_employees) +=1;
    }

    else
    {
      if(e->subordinates[i].n_subordinates != 0)
      {
        getListCount(&e->subordinates[i],name,n_employees);
      }

    }
  }
}

struct employee *getLast(struct employee *e, const char *name, size_t* n_employees, size_t* num, struct employee *ptr)
{
  if (ptr != NULL)
  {
    return ptr;
  }

  if ((strcmp(e->name,name)==0))
  {
    (*num) +=1;
  }

  if (e->subordinates >0)
  {
    for(int i = 0; i<e->n_subordinates;i++)
    {
      getLast(&e->subordinates[i],name,n_employees,num, ptr);
    }
  }

  if((*num) == (*n_employees) && (*num) >0)
  {
    ptr=e;
  }
  else
  {
    return NULL;
  }
}

void office_get_employees_at_level(struct office* office, size_t level,
  struct employee** emplys, size_t* n_employees)
  {

    if(emplys == NULL || n_employees == NULL)
    {
      return;
    }

    *emplys = malloc(sizeof(struct employee));
    // struct employee emp;
    struct employee  *ptr;
    // struct employee  **pptr;
    if(level == 0)
    {
      ptr = office->department_head;
      (*emplys)[0] = *ptr;
      (*emplys)[0].name = malloc(sizeof(char)*10);
      strcpy((*emplys)[0].name,office->department_head->name);
      *n_employees = 1;
    }

    if(level == 1)
    {
      for(int i = 0; i < office->department_head->n_subordinates;i++)
      {
        *emplys = realloc(*emplys,sizeof(struct employee)*10);
        ptr = &office->department_head->subordinates[i];
        (*emplys)[i] = *ptr;
        (*emplys)[i].name = malloc(sizeof(char)*10);


        strcpy((*emplys)[i].name, office->department_head->subordinates[i].name);
      }

      *n_employees = office->department_head->n_subordinates;

    }
    else if(level ==2)
    {


      int element = 0;
      for(int i = 0; i < office->department_head->n_subordinates;i++)
      {

        for(int j = 0; j < office->department_head->subordinates[i].n_subordinates;j++)
        {

          *emplys = realloc(*emplys,sizeof(struct employee)*10);
          ptr = &office->department_head->subordinates[i].subordinates[j];
          (*emplys)[element] = *ptr;
          (*emplys)[element].name = malloc(sizeof(char)*10);
          strcpy((*emplys)[element].name, office->department_head->subordinates[i].subordinates[j].name);
          element +=1;
        }
      }
      *n_employees = element;
    }

}




struct employee* office_get_last_employee_with_name(struct office* office,
  const char* name)
{

  size_t emps = 0;

  size_t num = 0;


  if(office == NULL || name == NULL)
  {
    return NULL;
  }



  getListCount(office->department_head,name,&emps);
  // printf("hello %zu\n", emps);


  struct employee *ptr = NULL;
  struct employee *e = getLast(office->department_head,name,&emps,&num,ptr);
  // printf("this is return: %zu\n",e);

  if(e == NULL)
  {
    return NULL;
  }
  else
  {

    return e;
  }



}




void office_get_employees_by_name(struct office* office, const char* name,
struct employee** emplys, size_t* n_employees)
{

  struct employee *list  = malloc(sizeof(struct employee));
  getbyNameList(office->department_head,name,n_employees,&list);
  size_t size = *n_employees;



  if(size != 0)
  {


    *emplys = malloc(sizeof(struct employee));
    struct employee  *ptr;
    for(int i = 0 ; i < size; i++ )
    {
      ptr = &list[i];
      *emplys = realloc(*emplys,sizeof(struct employee)*10);
      (*emplys)[i] = *ptr;
      (*emplys)[i].name = malloc(sizeof(char)*10);
      strcpy((*emplys)[i].name,list[i].name);
    }
  }

  free(list);
  list = NULL;

}

int hasChildren(struct employee *e)
{
  int hasChildren = 1;
  if (e->n_subordinates == 0)
  {
    hasChildren = 0;
  }
  else
  {
    hasChildren = 1;
  }
  return hasChildren;
}
//
int checkifinlist(int totalSize, struct employee**list, struct employee *e)
{
  int truth = 0;
  for(int i = 0; i < totalSize;i++)
  {

    if(strcmp((*list)[i].name,e->name)==0)
    {
      truth +=1;
    }
    else if (e->supervisor == NULL)
    {
      truth +=1;
    }
    else{
      truth +=0;
    }
  }
  return truth;
}
void addChildIntoList(struct employee *e, struct employee **list, size_t* n_employees )
{
  struct employee *ptr = NULL;

  if (e->n_subordinates > 0)
  {
    for (int i = 0; i< e->n_subordinates;i++)
    {
      if (hasChildren(&e->subordinates[i]) == 1)
      {
        //grand children found
        addChildIntoList(&e->subordinates[i], list, n_employees);
      }
    }


    //add first child into dep_list
    for (int j = 0; j< e->n_subordinates;j++)
    {

      ptr = &e->subordinates[j];
      int totalSize = (int)(*n_employees);
      int trues = checkifinlist(totalSize,list,ptr);
      if(trues == 0)
      {
        *list = realloc(*list, sizeof(struct employee)*30);
        (*list)[*n_employees] = *ptr;
        (*list)[*n_employees].name = malloc(sizeof(char)*10);
        strcpy((*list)[*n_employees].name,ptr->name);
        (*n_employees) +=1;
      }


    }


    int totalSize = (int)(*n_employees);
    int result = checkifinlist(totalSize,list,e);



    if(result == 0)
    {
      ptr = e;
      *list = realloc(*list, sizeof(struct employee)*30);
      (*list)[*n_employees] = *ptr;
      (*list)[*n_employees].name = malloc(sizeof(char)*10);
      strcpy((*list)[*n_employees].name,ptr->name);
      (*n_employees) +=1;
    }



  }


}


void office_get_employees_postorder(struct office* off,
  struct employee** emplys,
  size_t* n_employees)
  {
    if(emplys == NULL)
    {
      return;
    }
    *emplys = malloc(sizeof(struct employee));
    (*n_employees) = 0;
    addChildIntoList(off->department_head, emplys,n_employees);







}

void office_promote_employee(struct employee* emp)
{
  return;
}


void disbanding(struct employee *p)
  {
    if(p!= NULL)
    {

      if(p->n_subordinates>0)
      {


        for (int i = 0; i < p->n_subordinates; i++)
        {

          if(&p->subordinates[i]!=0)
          {
            // printf(" p name: %s and sub: %zu\n",p->subordinates[i].name, p->subordinates[i].n_subordinates);

            if(p->subordinates[i].n_subordinates > 0)
            {


              disbanding(&p->subordinates[i]);
            }

          }

        }



        for(int i = 0 ; i <p->n_subordinates; i++ )
        {
          if(&p->subordinates[i]!=0)
          {
            free(p->subordinates[i].name);
          }

        }
        free(p->subordinates);
        p->n_subordinates = 0;
      }
      else
      {
        return;
      }

    }


  }


void office_disband(struct office* office)
{

  if(office->department_head != NULL)
  {

    disbanding(office->department_head);

    free(office->department_head->name);
    free(office->department_head);
    free(office);
  }
  else
  {
    free(office);
  }
}

// //
// int main()
// {
//   struct office* off = malloc(sizeof(struct office));
//   off->department_head = NULL;
//
//   struct employee emp1 =
//   {
//     .name = "Emp1",
//     .supervisor = NULL,
//     .subordinates = NULL,
//     .n_subordinates = 0
//   };
//   office_employee_place(off,NULL,&emp1);
//
//
//   struct employee emp2 =
//   {
//     .name = "Emp2",
//     .supervisor = &emp1,
//     .subordinates = NULL,
//     .n_subordinates = 0
//   };
//   office_employee_place(off,&emp1,&emp2);
//
//   //
//   struct employee emp3 =
//   {
//     .name = "Emp3",
//     .supervisor = &emp1,
//     .subordinates = NULL,
//     .n_subordinates = 0
//   };
//   office_employee_place(off,&emp1,&emp3);
//
//   struct employee emp4 =
//   {
//     .name = "Emp4",
//     .supervisor = &emp1,
//     .subordinates = NULL,
//     .n_subordinates = 0
//   };
//   office_employee_place(off,&emp1,&emp4);
//
//   struct employee emp5 =
//   {
//     .name = "Emp5",
//     .supervisor = &emp1,
//     .subordinates = NULL,
//     .n_subordinates = 0
//   };
//   office_employee_place(off,&emp2,&emp5);
//
//   struct employee emp6 =
//   {
//     .name = "Emp5",
//     .supervisor = &emp1,
//     .subordinates = NULL,
//     .n_subordinates = 0
//   };
//   office_employee_place(off,&emp2,&emp6);
//
//   struct employee emp7 =
//   {
//     .name = "Emp5",
//     .supervisor = &emp1,
//     .subordinates = NULL,
//     .n_subordinates = 0
//   };
//   office_employee_place(off,&emp3,&emp7);
//
//   struct employee emp8 =
//   {
//     .name = "Emp6",
//     .supervisor = &emp1,
//     .subordinates = NULL,
//     .n_subordinates = 0
//   };
//   office_employee_place(off,&emp4,&emp8);
//
//   office_fire_employee(&off->department_head->subordinates[1]);
//   office_disband(off);
//
//
//
//
//
//   // struct employee *e = office_get_last_employee_with_name(off,"Emp5");
//   // printf("this is e: %zu\n",e);
// }
