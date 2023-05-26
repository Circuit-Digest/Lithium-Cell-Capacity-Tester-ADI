# Lithium-Cell-Capacity-Tester-ADI
In this project we have built a Li-ion cell capacity tester which can calculate the capacity of a lithium battery by charging it and then discharging it at a constant current. 

 ![image](https://github.com/Circuit-Digest/Lithium-Cell-Capacity-Tester-ADI/assets/65025308/b83bbcf3-c5cf-459a-8ad9-19e39dd5dc68)

From a simple hand-held trimmer to a super-fast electric car Lithium battery have found their way into a lot of applications in recent times. The voltage of a single lithium cell normally varies between 3.2V to 4.2V, it is often common to combine them to form a battery pack that can provide higher voltage and capacity. You can check out this article on Lithium-Ion Batteries if you are interested to know more about lithium cells and how they are used in a battery packs. In this article we will learning how to build a Lithium Cell Capacity Tester, special thanks to Digi-key for not only being a sponsor, but also for the timely shipping of all the required components used in this project. 



What is Cell Grading? And why do we need a Capacity Tester?

Previously we have discussed on how to build a 12V lithium battery pack using single 18650 cells. While the process seems to be pretty simple, things get pretty complicated when the number of cells used in a battery pack increases, mainly because each cell used in the battery pack should have the same capacity and similar IR (Internal Resistace). For example, consider a 12V 20Ah battery pack built using 5Ah cells. In this case we have to design a 4S3P battery pack consisting of 12 cells. Now even if one of these 12 cells are defective (has low capacity or discharges very fast) the whole battery pack will underperform. 

To filter out these defective cells and prevent them from getting into a battery pack, manufacturers do something called as Cell Grading. Wherein each cell is charged to its maximum voltage and then discharged at a constant current to calculate the actual capacity of the cell. Apart from this some Cell Grading machines will also measure the IR and temperature characteristics of the cell during the charging and discharging process. Whit this data, cells of similar capacity and IR are combined to gather and cells with lower capacity are rejected. 

For the purpose of this article we will be building a simple Lithium Cell Capacity testing device, which can charge the cell, discharge it at a preset constant current and calculate the capacity like shown below. 

 
![image](https://github.com/Circuit-Digest/Lithium-Cell-Capacity-Tester-ADI/assets/65025308/074492a0-30fb-4e9d-afca-3ce5ea2eb863)

