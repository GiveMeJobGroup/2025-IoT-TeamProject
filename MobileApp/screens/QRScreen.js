import React from 'react';
import { View, Text, StyleSheet, ImageBackground, Dimensions, TouchableOpacity } from 'react-native';
import QRCode from 'react-native-qrcode-svg';

export default function QRScreen({ route, navigation }) {
  const { qr_token } = route.params;

  const handleLogout = () => {
    // 필요 시 AsyncStorage.removeItem("token") 등 추가 가능
    navigation.replace('Login'); // 로그인 화면으로 이동
  };


  return (
    <ImageBackground
      source={{ uri: 'https://images.unsplash.com/photo-1607082349560-d0f37773f11c?auto=format&fit=crop&w=1470&q=80' }}
      style={styles.background}
      blurRadius={3}
    >
      <View style={styles.card}>
        <Text style={styles.title}>📱 나의 QR 코드</Text>

        <View style={styles.qrWrapper}>
          <QRCode
            value={qr_token}
            size={200}
          />
        </View>

        <Text style={styles.tokenLabel}>QR 코드 값</Text>
        <Text style={styles.tokenText}>{qr_token}</Text>

        <TouchableOpacity style={styles.logoutButton} onPress={handleLogout}>
          <Text style={styles.logoutText}>로그아웃</Text>
        </TouchableOpacity>
      </View>
    </ImageBackground>
  );
}

const { width } = Dimensions.get('window');

const styles = StyleSheet.create({
  background: {
    flex: 1,
    resizeMode: 'cover',
    justifyContent: 'center',
    alignItems: 'center',
  },
  card: {
    backgroundColor: 'rgba(255, 255, 255, 0.9)',
    padding: 25,
    borderRadius: 20,
    alignItems: 'center',
    width: width * 0.85,
    shadowColor: '#000',
    shadowOffset: { width: 0, height: 8 },
    shadowOpacity: 0.2,
    shadowRadius: 10,
    elevation: 5,
  },
  title: {
    fontSize: 26,
    fontWeight: 'bold',
    marginBottom: 20,
    color: '#333',
  },
  qrWrapper: {
    backgroundColor: '#fff',
    padding: 15,
    borderRadius: 15,
    marginBottom: 20,
    elevation: 3,
  },
  tokenLabel: {
    fontSize: 14,
    color: '#555',
  },
  tokenText: {
    fontSize: 12,
    color: '#777',
    marginTop: 5,
    textAlign: 'center',
  },
    logoutButton: {
    backgroundColor: '#c0392b',
    paddingVertical: 10,
    paddingHorizontal: 30,
    borderRadius: 8,
  },
  logoutText: {
    color: '#fff',
    fontWeight: '600',
    fontSize: 16,
  },
});
