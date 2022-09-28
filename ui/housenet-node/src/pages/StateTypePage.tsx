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

export default function StateTypePage(props:PageProps) {
    
    const [configuration, setConfiguration] = useState<any>({});
  
    function refresh()  {

      fetch(`${Configuration.baseUrl}/housenet/elements/state/${props.id}`)           
      .then(res => res.json())
      .then(data => {
        setConfiguration(data);
     
      });
    }

    useEffect( () => {
      refresh();
      const timer = setInterval(() => refresh(), 750);
      return () => clearTimeout(timer);
    }, []);

    return (
     <div>
       <h1>State</h1>
       <h2>{props.id}</h2>
       <p>On/Off value based on a pin being pulled to ground</p>
       <Table size="small">
        <TableHead>
          
        </TableHead>
        <TableBody>         
            <TableRow >
              <TableCell>Pin</TableCell>
              <TableCell>{props.config?.pin}</TableCell>
            </TableRow>          
            <TableRow >
              <TableCell>State</TableCell>
              <TableCell>{configuration.state?"ACTIVE": "inactive"} </TableCell>
            </TableRow>  
        </TableBody>
      </Table>
       
      </div>
        
    );
}
