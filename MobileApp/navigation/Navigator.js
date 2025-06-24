import React from 'react';
import { createNativeStackNavigator } from '@react-navigation/native-stack';
import LoginScreen from '../screens/LoginScreen';
import RegisterScreen from '../screens/RegisterScreen';
import HomeScreen from '../screens/HomeScreen';
import QRScreen from '../screens/QRScreen';

const Stack = createNativeStackNavigator();

export default function AuthNavigator() {
  return (
    <Stack.Navigator initialRouteName="Login">
      <Stack.Screen name="Login" component={LoginScreen} options={{ headerShown: false }}/>
      <Stack.Screen name="Register" component={RegisterScreen} options={{ headerShown: false }} />
      <Stack.Screen name="QR" component={QRScreen} options={{ headerLeft: null, headerShown: false }} />
      <Stack.Screen name="Home" component={HomeScreen} options={{ headerLeft: null, headerShown: false }} />
    </Stack.Navigator>
  );
}
