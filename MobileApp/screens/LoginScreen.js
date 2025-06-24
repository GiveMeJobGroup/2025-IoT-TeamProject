// screens/LoginScreen.js
import React, { useState } from 'react';
import {
  View,
  Text,
  TextInput,
  StyleSheet,
  TouchableOpacity,
  ImageBackground,
  KeyboardAvoidingView,
  Platform,
} from 'react-native';
import { Ionicons } from '@expo/vector-icons';

export default function LoginScreen({ navigation }) {
  const [email, setEmail] = useState('');
  const [password, setPassword] = useState('');

  const handleLogin = async () => {
    try {
      // 로그인 요청
      const res = await fetch('http://210.119.12.64:5000/login', {
        method: 'POST',
        headers: { 'Content-Type': 'application/json' },
        body: JSON.stringify({ email, password }),
      });

      const data = await res.json();

      if (res.ok) {
        alert(data.message); // 로그인 성공 메시지
        navigation.replace('QR', { qr_token: data.qr_token }); // 홈 화면으로 이동, qr} else {
        alert(data.message || '로그인 실패');
      }
    } catch (err) {
      console.error(err); 
      alert('서버 연결에 실패했습니다.');
    }
  };

  return (
    <ImageBackground
      source={{ uri: 'https://encrypted-tbn0.gstatic.com/images?q=tbn:ANd9GcRjFL3-jwGmb8O6GJ0kgolrJuoGXahDTyVt-Q&s' }}
      style={styles.background}
      blurRadius={3}
    >
      <KeyboardAvoidingView
        behavior={Platform.OS === 'ios' ? 'padding' : undefined}
        style={styles.container}
      >
        <Text style={styles.title}>Dragon Sugar L.R</Text>

        <View style={styles.inputContainer}>
          <Ionicons name="mail-outline" size={20} color="#333" style={styles.icon} />
          <TextInput
            placeholder="이메일"
            value={email}
            onChangeText={setEmail}
            style={styles.input}
            keyboardType="email-address"
          />
        </View>

        <View style={styles.inputContainer}>
          <Ionicons name="lock-closed-outline" size={20} color="#333" style={styles.icon} />
          <TextInput
            placeholder="비밀번호"
            value={password}
            onChangeText={setPassword}
            secureTextEntry
            style={styles.input}
          />
        </View>

        <TouchableOpacity style={styles.loginButton} onPress={handleLogin}>
          <Text style={styles.loginText}>로그인</Text>
        </TouchableOpacity>

        <TouchableOpacity onPress={() => navigation.navigate('Register')}>
          <Text style={styles.link}>계정이 없으신가요? 회원가입</Text>
        </TouchableOpacity>
      </KeyboardAvoidingView>
    </ImageBackground>
  );
}

const styles = StyleSheet.create({
  background: {
    flex: 1,
    resizeMode: 'cover',
    justifyContent: 'center',
  },
  container: {
    padding: 25,
    backgroundColor: 'rgba(255,255,255,0.8)',
    margin: 20,
    borderRadius: 15,
  },
  title: {
    fontSize: 28,
    fontWeight: '700',
    textAlign: 'center',
    marginBottom: 30,
    color: '#333',
  },
  inputContainer: {
    flexDirection: 'row',
    alignItems: 'center',
    borderColor: '#aaa',
    borderWidth: 1,
    borderRadius: 8,
    marginBottom: 15,
    paddingHorizontal: 10,
    backgroundColor: '#fff',
  },
  icon: {
    marginRight: 8,
  },
  input: {
    flex: 1,
    height: 45,
    fontSize: 16,
  },
  loginButton: {
    backgroundColor: '#4a7c59',
    paddingVertical: 12,
    borderRadius: 8,
    marginTop: 10,
  },
  loginText: {
    color: '#fff',
    textAlign: 'center',
    fontSize: 17,
    fontWeight: '600',
  },
  link: {
    marginTop: 20,
    textAlign: 'center',
    color: '#333',
    textDecorationLine: 'underline',
  },
});
