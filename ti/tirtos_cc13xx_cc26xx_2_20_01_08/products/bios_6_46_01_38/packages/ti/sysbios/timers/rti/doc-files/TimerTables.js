
var m_count = 0;
m_items = new Array(
"<h3>ti.catalog.arm.cortexr4</h3>",
"<h5>AR14XX</h5>",
"<table border=1 cellpadding=3>",
 "<colgroup span=1></colgroup> <colgroup span=5 align=center></colgroup>",
   "<tr><th> Timer ID </th><th> Timer Name </th><th> Timer Base Address </th><th> Timer Interrupt Number </th><th> Timer Event Id </th></tr>",
   "<tr><td> 0        </td><td> RTI Timer0   </td><td> 0xfffffc00         </td><td> 2         </td><td> Not used         </td></tr>",
   "<tr><td> 1        </td><td> RTI Timer1   </td><td> 0xfffffc00         </td><td> 3         </td><td> Not used         </td></tr>",
 "</table>",
"<h5>AR16XX</h5>",
"<table border=1 cellpadding=3>",
 "<colgroup span=1></colgroup> <colgroup span=5 align=center></colgroup>",
   "<tr><th> Timer ID </th><th> Timer Name </th><th> Timer Base Address </th><th> Timer Interrupt Number </th><th> Timer Event Id </th></tr>",
   "<tr><td> 0        </td><td> RTI Timer0   </td><td> 0xfffffc00         </td><td> 2         </td><td> Not used         </td></tr>",
   "<tr><td> 1        </td><td> RTI Timer1   </td><td> 0xfffffc00         </td><td> 3         </td><td> Not used         </td></tr>",
 "</table>",
"<h5>RM48L.*</h5>",
"<table border=1 cellpadding=3>",
 "<colgroup span=1></colgroup> <colgroup span=5 align=center></colgroup>",
   "<tr><th> Timer ID </th><th> Timer Name </th><th> Timer Base Address </th><th> Timer Interrupt Number </th><th> Timer Event Id </th></tr>",
   "<tr><td> 0        </td><td> RTI Timer0   </td><td> 0xfffffc00         </td><td> 2         </td><td> Not used         </td></tr>",
   "<tr><td> 1        </td><td> RTI Timer1   </td><td> 0xfffffc00         </td><td> 3         </td><td> Not used         </td></tr>",
 "</table>",
"<h3>ti.catalog.c6000</h3>",
"<h5>AR16XX</h5>",
"<table border=1 cellpadding=3>",
 "<colgroup span=1></colgroup> <colgroup span=5 align=center></colgroup>",
   "<tr><th> Timer ID </th><th> Timer Name </th><th> Timer Base Address </th><th> Timer Interrupt Number </th><th> Timer Event Id </th></tr>",
   "<tr><td> 0        </td><td> RTI2 Timer0   </td><td> 0x2020000         </td><td> 14         </td><td> 75         </td></tr>",
   "<tr><td> 1        </td><td> RTI2 Timer1   </td><td> 0x2020000         </td><td> 15         </td><td> 76         </td></tr>",
 "</table>",
"<!-- -->"
);

// gen below code only 1 time
while (m_count < m_items.length) {
    document.write(m_items[m_count]);
    m_count++;
}