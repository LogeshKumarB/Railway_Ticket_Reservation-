#include <iostream>
#include <map>
#include <queue>
#include <string.h>
#include <list>
using namespace std;
class Passenger
{
public:
    int id = 1; // static variable to give id for every new passenger
    string name;
    int age;
    string berthPreference; // U or L or M
    int passengerId;        // id of passenger created automatically
    string alloted;         // alloted type (L,U,M,RAC,WL)
    int number;             // seat number
    Passenger(string name, int age, string berthPreference)
    {
        this->name = name;
        this->age = age;
        this->berthPreference = berthPreference;
        this->passengerId = id++;
        alloted = "";
        number = -1;
    }
};

class TicketBooker
{
    // 63 berths(upper ,lower , middle)  + ( 18 RAC passengers)
    // 10 waiting list tickets ->21 L, 21 M, 21U , 18RAC, 10WL
public:
    int availableLowerBerths = 1;  // normally 21
    int availableMiddleBerths = 1; // normally 21
    int availableUpperBerths = 1;  // normally 21
    int availableRacTickets = 1;   // normally 18
    int availableWaitingList = 1;  // normally 10

    queue<int> waitingList;     // = new LinkedList<>();//queue of WL passengers
    queue<int> racList;         // =  new LinkedList<>();//queu of RAC passengers
    list<int> bookedTicketList; // =  new ArrayList<>();//list of bookedticket passengers

    list<int> lowerBerthsPositions = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21};  // = new ArrayList<>(Arrays.asList(1));//normally 1,2,...21
    list<int> middleBerthsPositions = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21}; // new ArrayList<>(Arrays.asList(1));//normally 1,2,...21
    list<int> upperBerthsPositions = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21};  // new ArrayList<>(Arrays.asList(1));//normally 1,2,...21
    list<int> racPositions = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18};                      //= new ArrayList<>(Arrays.asList(1));//normally 1,2,...18
    list<int> waitingListPositions = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};                                              // new ArrayList<>(Arrays.asList(1));//normally 1,2,...10
    map<int, Passenger> passengers;                                                                                // = new HashMap<>();//map of passenger ids to passengers

    // book ticket
    void bookTicket(Passenger p, int berthInfo, string allotedBerth)
    {
        // assign the seat number and type of berth(L,U,M)
        p.number = berthInfo;
        p.alloted = allotedBerth;
        // add passenger to the map
        passengers.insert({p.passengerId, p});
        // add passenger id to the list of booked tickets
        bookedTicketList.push_back(p.passengerId);
        cout << "--------------------------Booked Successfully";
    }

    // adding to RAC
    void addToRAC(Passenger p, int racInfo, string allotedRAC)
    {
        // assign seat number and type(RAC)
        p.number = racInfo;
        p.alloted = allotedRAC;
        // add passenger to the map
        passengers.insert({p.passengerId, p});
        // add passenger id to the queue of RAC tickets
        racList.push(p.passengerId);
        // decrease available RAC tickets by 1
        availableRacTickets--;
        // remove the position that was alloted to the passenger
        racPositions.erase(racPositions.begin());

        cout << "--------------------------added to RAC Successfully";
    }

    // adding to WL
    void addToWaitingList(Passenger p, int waitingListInfo, string allotedWL)
    {
        // assign seat number and type(WL)
        p.number = waitingListInfo;
        p.alloted = allotedWL;
        // add passenger to the map
        passengers.insert({p.passengerId, p});
        // add passenger id to the queue of WL tickets
        waitingList.push(p.passengerId);
        // decrease available WL tickets by 1
        availableWaitingList--;
        // remove the position that was alloted to the passenger
        waitingListPositions.erase(waitingListPositions.begin());

        cout << "-------------------------- added to Waiting List Successfully";
    }

    // cancel ticket
    /* public void cancelTicket(int passengerId)
     {
         //remove the passenger from the map
         Passenger p = passengers.get(passengerId);
         passengers.remove(Integer.valueOf(passengerId));
         //remove the booked ticket from the list
         bookedTicketList.remove(Integer.valueOf(passengerId));

         //take the booked position which is now free
         int positionBooked = p.number;

         System.out.println("---------------cancelled Successfully");

         //add the free position to the correspoding type of list (either L,M,U)
         if(p.alloted.equals("L"))
         {
           availableLowerBerths++;
           lowerBerthsPositions.add(positionBooked);
         }
         else if(p.alloted.equals("M"))
         {
           availableMiddleBerths++;
           middleBerthsPositions.add(positionBooked);
         }
         else if(p.alloted.equals("U"))
         {
           availableUpperBerths++;
           upperBerthsPositions.add(positionBooked);
         }

         //check if any RAC is there
         if(racList.size() > 0)
         {
             //take passenger from RAC and increase the free space in RAC list and increase available RAC tickets
             Passenger passengerFromRAC = passengers.get(racList.poll());
             int positionRac = passengerFromRAC.number;
             racPositions.add(positionRac);
             racList.remove(Integer.valueOf(passengerFromRAC.passengerId));
             availableRacTickets++;

             //check if any WL is there
             if(waitingList.size()>0)
             {
                 //take the passenger from WL and add them to RAC , increase the free space in waiting list and
                 //increase available WL and decrease available RAC by 1
                 Passenger passengerFromWaitingList = passengers.get(waitingList.poll());
                 int positionWL = passengerFromWaitingList.number;
                 waitingListPositions.add(positionWL);
                 waitingList.remove(Integer.valueOf(passengerFromWaitingList.passengerId));

                 passengerFromWaitingList.number = racPositions.get(0);
                 passengerFromWaitingList.alloted = "RAC";
                 racPositions.remove(0);
                 racList.add(passengerFromWaitingList.passengerId);

                 availableWaitingList++;
                 availableRacTickets--;
             }
             // now we have a passenger from RAc to whom we can book a ticket,
             //so book the cancelled ticket to the RAC passenger
             Main.bookTicket(passengerFromRAC);
         }

     }*/

    // print all available seats
    void printAvailable()
    {
        cout << "Available Lower Berths " << availableLowerBerths;
        cout << "Available Middle Berths " << availableMiddleBerths;
        cout << "Available Upper Berths " << availableUpperBerths;
        cout << "Availabel RACs " << availableRacTickets;
        cout << "Available Waiting List " << availableWaitingList;
        cout << "--------------------------";
    }

    // print all occupied passengers from all types including WL
    /* void printPassengers()
    {
        if(passengers.size() == 0)
        {
            cout<<"No details of passengers";
            return;
        }
        for(auto Passenger p : passengers.values())
        {
            System.out.println("PASSENGER ID " + p.passengerId );
            System.out.println(" Name " + p.name );
            System.out.println(" Age " + p.age );
            System.out.println(" Status " + p.number + p.alloted);
            System.out.println("--------------------------");
        }
    }*/
};

// function for bokoing ticket
void bookTicket(Passenger p)
{
    TicketBooker TicketBooker;
    // if no WL is available , means all tickets are filled.. so no tickets available
    if (TicketBooker.availableWaitingList == 0)
    {
        cout << "No Tickets Available";
        return;
    }
    // check if preferred berth is available
    if ((p.berthPreference == "L" && TicketBooker.availableLowerBerths > 0) ||
        (p.berthPreference == "M" && TicketBooker.availableMiddleBerths > 0) ||
        (p.berthPreference == "U" && TicketBooker.availableUpperBerths > 0))
    {
        cout << "Preferred Berth Available";
        if (p.berthPreference == "L")
        {
            cout << "Lower Berth Given";
            // call booking function in the TicketBooker class
            TicketBooker.bookTicket(p, (TicketBooker.lowerBerthsPositions.front()), "L");
            // remove the booked position from available positions and also decrease available seats of that
            //  particular type
            TicketBooker.lowerBerthsPositions.erase(TicketBooker.lowerBerthsPositions.begin());
            TicketBooker.availableLowerBerths--;
        }
        else if (p.berthPreference == "M")
        {
            cout << "MIDDLE Berth Given";
            // call booking function in the TicketBooker class
            TicketBooker.bookTicket(p, (TicketBooker.middleBerthsPositions.front()), "M");
            // remove the booked position from available positions and also decrease available seats of that
            //  particular type
            TicketBooker.middleBerthsPositions.erase(TicketBooker.middleBerthsPositions.begin());
            TicketBooker.availableMiddleBerths--;
        }
        else if (p.berthPreference == "U")
        {
            cout << "Upper Berth Given";
            // call booking function in the TicketBooker class
            TicketBooker.bookTicket(p, (TicketBooker.upperBerthsPositions.front()), "U");
            // remove the booked position from available positions and also decrease available seats of that
            //  particular type
            TicketBooker.upperBerthsPositions.erase(TicketBooker.upperBerthsPositions.begin());
            TicketBooker.availableUpperBerths--;
        }
    }
    // preference not available -> book the available berth

    else if (TicketBooker.availableLowerBerths > 0)
    {
        cout << "Lower Berth Given";
        // call booking function in the TicketBooker class
        TicketBooker.bookTicket(p, (TicketBooker.lowerBerthsPositions.front()), "L");
        // remove the booked position from available positions and also decrease available seats of that
        //  particular type
        TicketBooker.lowerBerthsPositions.erase(TicketBooker.lowerBerthsPositions.begin());
        TicketBooker.availableLowerBerths--;
    }
    else if (TicketBooker.availableMiddleBerths > 0)
    {
        cout << "Middle Berth Given";
        // call booking function in the TicketBooker class
        TicketBooker.bookTicket(p, (TicketBooker.middleBerthsPositions.front()), "M");
        // remove the booked position from available positions and also decrease available seats of that
        //  particular type
        TicketBooker.middleBerthsPositions.erase(TicketBooker.middleBerthsPositions.begin());
        TicketBooker.availableMiddleBerths--;
    }
    else if (TicketBooker.availableUpperBerths > 0)
    {
        cout << "Upper Berth Given";
        // call booking function in the TicketBooker class
        TicketBooker.bookTicket(p, (TicketBooker.upperBerthsPositions.front()), "U");
        // remove the booked position from available positions and also decrease available seats of that
        //  particular type
        TicketBooker.upperBerthsPositions.erase(TicketBooker.upperBerthsPositions.begin());
        TicketBooker.availableUpperBerths--;
    }
    // if no berth available go to RAC
    else if (TicketBooker.availableRacTickets > 0)
    {
        cout << "RAC available";
        TicketBooker.addToRAC(p, (TicketBooker.racPositions.front()), "RAC");
    }
    // if no RAC available go to WL
    else if (TicketBooker.availableWaitingList > 0)
    {
        cout << "Added to Waiting List";
        TicketBooker.addToWaitingList(p, (TicketBooker.waitingListPositions.front()), "WL");
    }
}
// cancel ticket function
/* public static void cancelTicket(int id)
 {
     TicketBooker booker = new TicketBooker();
     //check if passenger id is valid
     if(!booker.passengers.containsKey(id))
     {
         System.out.println("Passenger detail Unknown");
     }
     else
         booker.cancelTicket(id);
 }*/
int main()
{
    // Scanner s = new Scanner(System.in);
    bool loop = true;
    // loop to g<(" 1. Book Ticket \n 2. Cancel Ticket \n 3. Available Tickets \n 4. Booked Tickets \n 5. Exit";
    int choice;
    // Passenger p;
    cin >> choice;
    switch (choice)
    {
    // book ticket
    case 1:
    {
        // get details from Passenger
        cout << "Enter Passenger name,age and berth preference (L,M or U)";
        string name, berthPreference;
        int age;
        cin >> name;
        cin >> age;
        cin >> berthPreference; // = s.nextInt();
        // get berth preference (L,U,M)
        // String berthPreference = s.next();
        // create a passenger object
        Passenger p(name, age, berthPreference);
        // booking
        bookTicket(p);
    }
    break;
    // cancel ticket
    case 2:
    {
        // get passenger id to cancel
        cout << "Enter passenger Id to cancel";
        int id;
        cin >> id;
        cout << id << "do later";

        // cancelTicket(id);
    }
    break;
    // available tickets print
    case 3:
    {
        TicketBooker booker;
        booker.printAvailable();
    }
    break;
    // occupied tickets print
    case 4:
    {
        TicketBooker booker;
        // booker.printPassengers();
    }
    break;
    // exit
    case 5:
    {
        loop = false;
    }
    break;
    default:
        break;
    }
}