// примесь
let sayHiMixin = {
    sayHi() {
      alert(`Привет, ${this.name}`);
    },
    sayBye() {
      alert(`Пока, ${this.name}`);
    }
  };
  
  // использование:
  class User {
    constructor(name) {
      this.name = name;
    }
  }
  
  // копируем методы
  Object.assign(User.prototype, sayHiMixin);
  
  // теперь User может сказать Привет
  new User("Вася").sayHi(); // Привет, Вася!
  console.log(2);