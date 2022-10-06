import { Table, TableBody, TableCell, TableHead, TableRow, TextField } from "@mui/material";
import { useEffect, useState } from "react";
import Configuration  from '../Config';

// {"ip":"192.168.2.103",
// "version":"1.2.0",
// "build":"Mar 13 2022 14:00:18","sha1":"afec3753fec0d4dc76ddd02dccae08d57e265451","free":242320,"processing":true,"uptime_mins":429499}

interface PageProps {
  id: string,
  config:any
}

export default function EMonTXPage(props:PageProps) {
    
    const [configuration, setConfiguration] = useState<any>({});
  
    function refresh()  {

      fetch(`${Configuration.baseUrl}/housenet/elements/emontx/${props.id}`)           
      .then(res => res.json())
      .then(data => {
        setConfiguration(data);
     
      });
    }

    useEffect( () => {
      refresh();     
    }, []);

    return (
     <div>
       <h1>EMonTX</h1>
       <h2>{props.id}</h2>
       <p>Connection to EmonTX device to republish over MQTT</p>
       
       

       <Table size="small">
        <TableHead>
        <TableRow >
            <TableCell>Power</TableCell>
            <TableCell>In</TableCell>
            <TableCell>Out</TableCell>
          </TableRow> 
        </TableHead>
        <TableBody>    

        {configuration?.channels?.map( (e:any) => {
          return ( <TableRow >
            <TableCell>{e.power}</TableCell>
            <TableCell>{e.in}</TableCell>
            <TableCell>{e.out}</TableCell>
          </TableRow>         )}
          )
        }

            
         
        </TableBody>
      </Table>

      <Table size="small">
       
        <TableBody>    
        <TableRow >
      Sequence: {configuration?.seq}
      </TableRow>
      <TableRow>
      Voltage: {configuration?.v}
      </TableRow>
      </TableBody>
      </Table>
        </div>
    );
}
