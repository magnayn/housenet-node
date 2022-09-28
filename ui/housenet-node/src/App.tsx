import * as React from 'react';
import { styled, createTheme, ThemeProvider } from '@mui/material/styles';
import CssBaseline from '@mui/material/CssBaseline';
import MuiDrawer from '@mui/material/Drawer';
import Box from '@mui/material/Box';
import MuiAppBar, { AppBarProps as MuiAppBarProps } from '@mui/material/AppBar';
import Toolbar from '@mui/material/Toolbar';
import List from '@mui/material/List';
import Typography from '@mui/material/Typography';
import Divider from '@mui/material/Divider';
import IconButton from '@mui/material/IconButton';
import Badge from '@mui/material/Badge';
import Container from '@mui/material/Container';
import Grid from '@mui/material/Grid';
import Paper from '@mui/material/Paper';
import Link from '@mui/material/Link';
import MenuIcon from '@mui/icons-material/Menu';
import ChevronLeftIcon from '@mui/icons-material/ChevronLeft';
import NotificationsIcon from '@mui/icons-material/Notifications';
import { mainListItems, secondaryListItems } from './listitems';
import StatusPage from './pages/StatusPage';
import ConfigPage from './pages/ConfigPage';
import { ListItemButton, ListItemIcon, ListItemText } from '@mui/material';
// import Chart from './Chart';
// import Deposits from './Deposits';
// import Orders from './Orders';
import DashboardIcon from '@mui/icons-material/Dashboard';
import ShoppingCartIcon from '@mui/icons-material/ShoppingCart';
import PeopleIcon from '@mui/icons-material/People';
import BarChartIcon from '@mui/icons-material/BarChart';
import LayersIcon from '@mui/icons-material/Layers';
import AssignmentIcon from '@mui/icons-material/Assignment';
import { ElectricMeterOutlined, MoveToInboxOutlined, PowerSettingsNew, SettingsOutlined, SettingsRemoteTwoTone } from '@mui/icons-material';
import MeterTypePage from './pages/MeterTypePage';
import StateTypePage from './pages/StateTypePage';
import UpdateFirmwarePage from './pages/UpdateFirmwarePage';
import Configuration from './Config';
import EMonTXPage from './pages/EmonTXPage';
import OnewirePage from './pages/OnewirePage';
import OpenthermPage from './pages/OpenthermPage';

const drawerWidth: number = 240;

interface AppBarProps extends MuiAppBarProps {
  open?: boolean;
}

const AppBar = styled(MuiAppBar, {
  shouldForwardProp: (prop) => prop !== 'open',
})<AppBarProps>(({ theme, open }) => ({
  zIndex: theme.zIndex.drawer + 1,
  transition: theme.transitions.create(['width', 'margin'], {
    easing: theme.transitions.easing.sharp,
    duration: theme.transitions.duration.leavingScreen,
  }),
  ...(open && {
    marginLeft: drawerWidth,
    width: `calc(100% - ${drawerWidth}px)`,
    transition: theme.transitions.create(['width', 'margin'], {
      easing: theme.transitions.easing.sharp,
      duration: theme.transitions.duration.enteringScreen,
    }),
  }),
}));

const Drawer = styled(MuiDrawer, { shouldForwardProp: (prop) => prop !== 'open' })(
  ({ theme, open }) => ({
    '& .MuiDrawer-paper': {
      position: 'relative',
      whiteSpace: 'nowrap',
      width: drawerWidth,
      transition: theme.transitions.create('width', {
        easing: theme.transitions.easing.sharp,
        duration: theme.transitions.duration.enteringScreen,
      }),
      boxSizing: 'border-box',
      ...(!open && {
        overflowX: 'hidden',
        transition: theme.transitions.create('width', {
          easing: theme.transitions.easing.sharp,
          duration: theme.transitions.duration.leavingScreen,
        }),
        width: theme.spacing(7),
        [theme.breakpoints.up('sm')]: {
          width: theme.spacing(9),
        },
      }),
    },
  }),
);

const mdTheme = createTheme();

interface ElementsProps {
  items: any[];
  onClick?: Function;
}

function ElementsList(props: ElementsProps) {


  function click(item: any) {
    if (props.onClick != null)
      props.onClick(item);
  }

  return (<div>
    {props.items.map(e => {
      return (
        <ListItemButton onClick={() => click(e)} key={e.id}>
          <ListItemIcon>
            {e.type == 'meter' ? <ElectricMeterOutlined/> : <DashboardIcon />}
            
          </ListItemIcon>
          <ListItemText primary={e.type} secondary={e.id} />
        </ListItemButton>)
    })} </div>);


}

function DashboardContent() {

  const [page, setPage] = React.useState("status");
  const [item, setItem] = React.useState<any>();

  const [open, setOpen] = React.useState(true);

  const [configuration, setConfiguration] = React.useState<any>();

  React.useEffect(() => {

    fetch(`${Configuration.baseUrl}/housenet/config`)
      .then(res => res.json())
      .then(data => setConfiguration(data));
  }, []);

  const toggleDrawer = () => {
    setOpen(!open);
  };

  function goto(page: string) {
    setPage(page);
  }

  function selectItem(item: any) {

    console.log("SelectItem ");
    console.log(item);


    setPage(item.type);
    setItem(item);
  }

  return (
    <ThemeProvider theme={mdTheme}>
      <Box sx={{ display: 'flex' }}>
        <CssBaseline />
        <AppBar position="absolute" open={open}>
          <Toolbar
            sx={{
              pr: '24px', // keep right padding when drawer closed
            }}
          >
            <IconButton
              edge="start"
              color="inherit"
              aria-label="open drawer"
              onClick={toggleDrawer}
              sx={{
                marginRight: '36px',
                ...(open && { display: 'none' }),
              }}
            >
              <MenuIcon />
            </IconButton>
            <Typography
              component="h1"
              variant="h6"
              color="inherit"
              noWrap
              sx={{ flexGrow: 1 }}
            >
              HouseNet Node
            </Typography>
            {/* <IconButton color="inherit">
              <Badge badgeContent={4} color="secondary">
                <NotificationsIcon />
              </Badge>
            </IconButton> */}
          </Toolbar>
        </AppBar>
        <Drawer variant="permanent" open={open}>
          <Toolbar
            sx={{
              display: 'flex',
              alignItems: 'center',
              justifyContent: 'flex-end',
              px: [1],
            }}
          >
            <IconButton onClick={toggleDrawer}>
              <ChevronLeftIcon />
            </IconButton>
          </Toolbar>
          <Divider />
          <List component="nav">

            <ListItemButton onClick={() => goto('status')} key="status">
              <ListItemIcon>
                <PowerSettingsNew />
              </ListItemIcon>
              <ListItemText primary="Status" />
            </ListItemButton>

            <ListItemButton onClick={() => goto('firmware')}  key="firmware">
              <ListItemIcon>
                <MoveToInboxOutlined />
              </ListItemIcon>
              <ListItemText primary="Firmware" />
            </ListItemButton>

            <ListItemButton onClick={() => goto('config')}  key="config">
              <ListItemIcon>
                <SettingsOutlined />
              </ListItemIcon>
              <ListItemText primary="Config" />
            </ListItemButton>

            <Divider sx={{ my: 1 }} />
            {configuration?.elements?.length > 0 && (
              <ElementsList items={configuration?.elements}
                onClick={(item: any) => selectItem(item)} />)}



          </List>
        </Drawer>
        <Box
          component="main"
          sx={{
            backgroundColor: (theme) =>
              theme.palette.mode === 'light'
                ? theme.palette.grey[100]
                : theme.palette.grey[900],
            flexGrow: 1,
            height: '100vh',
            overflow: 'auto',
          }}
        >
          <Toolbar />
          <Container id="c" maxWidth="lg" sx={{ mt: 4, mb: 4, flexGrow: 1,
            overflow: 'auto'
            }} >

            {page == "status" && <StatusPage />}
            {page == "config" && <ConfigPage />}
            {page == "firmware" && <UpdateFirmwarePage />}
            {page == "state" && <StateTypePage key={item.id} id={item.id} config={item} />}
            {page == "meter" && <MeterTypePage id={item.id} config={item} />}
            {page == "emontx" && <EMonTXPage id={item.id} config={item} />}
            {page == "onewire" && <OnewirePage id={item.id} config={item} />}
            {page == "opentherm" && <OpenthermPage id={item.id} config={item} />}

          </Container>
        </Box>
      </Box>
    </ThemeProvider>
  );
}

export default function App() {
  return <DashboardContent />;
}